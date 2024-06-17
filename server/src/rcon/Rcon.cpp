// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "rcon/Rcon.hpp"

#include "Logger.hpp"
#include "Server.hpp"

tetriq::RconClient::RconClient(int socket, sockaddr_in address)
    : _socket(socket)
    , _address(address)
{}

tetriq::RconClient::~RconClient()
{
    close(_socket);
}

int tetriq::RconClient::getSocket() const
{
    return _socket;
}

tetriq::RconClient &tetriq::RconClient::operator<<(const std::string &response)
{
    _res_queue.push(response);
    return *this;
}

void tetriq::Rcon::addCommands()
{
    this->registerCommand("startgame", &ServerManager::_startGame);
    this->registerCommand("stopgame", &ServerManager::_stopGame);
    this->registerCommand("get", &ServerManager::_get);
    this->registerCommand("set", &ServerManager::_set);
    this->registerCommand("list", &ServerManager::_list);
    this->registerCommand("create", &ServerManager::_create);
    this->registerCommand("delete", &ServerManager::_delete);
    _command_handler.registerCommand("help",
        std::make_unique<
            std::function<void(const std::vector<std::string> &, std::queue<std::string> &)>>(
            [this](const std::vector<std::string> &, std::queue<std::string> &) {
                std::string res = "RCON: Available commands:\n";
                for (const auto &[command, _] : _command_handler.getCommands()) {
                    res += command + "\n";
                }
                *this->_client << res;
            }));
}

tetriq::Rcon::Rcon(Server &server)
    : _server(server)
    , _config(_server.getConfig().rcon)
    , _rcon_socket()
    , _rcon_port()
    , _rcon_address()
{
    if (_config.enabled == false) {
        RCONLOG(INFO) << "Rcon disabled" << std::endl;
        return;
    }
    _readfds = {};
    RCONLOG(INFO) << "Rcon created" << std::endl;
    RCONLOG(INFO) << "Password: " << _config.password << std::endl;
    addCommands();
}

tetriq::Rcon::~Rcon()
{
    if (_config.enabled == false) {
        return;
    }
    _is_running = false;
    close(_rcon_socket);
    RCONLOG(INFO) << "Rcon destroyed" << std::endl;
}

void tetriq::Rcon::handleNewClient()
{
    if (FD_ISSET(_rcon_socket, &_readfds)) {
        sockaddr_in new_client_addr{};
        socklen_t addrlen = sizeof(new_client_addr);
        int new_client_socket =
            ::accept(_rcon_socket, (struct sockaddr *) &new_client_addr, &addrlen);
        if (new_client_socket < 0) {
            RCONLOG(ERROR) << "Accept error" << std::endl;
        }
        if (_client != nullptr) {
            std::string msg = "Maximum connections reached";
            send(new_client_socket, msg.c_str(), msg.size(), 0);
            ::close(new_client_socket);
            return;
        }
        _client = std::make_unique<RconClient>(new_client_socket, new_client_addr);
        *_client << "RCON: Enter password\n";
        RCONLOG(INFO) << "New connection" << std::endl;
    }
}

void tetriq::Rcon::disconnectClient()
{
    constexpr char msg[] = "RCON: Connection closed\n";
    ::send(_client->getSocket(), msg, sizeof(msg), 0);
    ::close(_client->getSocket());
    _client = nullptr;
    RCONLOG(INFO) << "Connection closed" << std::endl;
}

void tetriq::Rcon::parseCmd(
    std::istringstream &iss, std::string &command, std::vector<std::string> &arguments)
{
    std::getline(iss, command, ' ');

    std::string arg;
    while (std::getline(iss, arg, ' ')) {
        arguments.push_back(arg);
    }
}

bool tetriq::Rcon::checkAuth(std::string command)
{
    if (_client->_is_authenticated == false) {
        if (command == _config.password) {
            _client->_is_authenticated = true;
            *_client << "RCON: Authenticated\n";
            return true;
        }
        *_client << "RCON: Not authenticated\n";
        return true;
    }
    return false;
}

void tetriq::Rcon::sendRes()
{
    if (FD_ISSET(_client->getSocket(), &_writefds)) {
        while (not _client->_res_queue.empty()) {
            std::string response = _client->_res_queue.front();
            _client->_res_queue.pop();
            ::send(_client->getSocket(), response.c_str(), response.size(), 0);
        }
        FD_CLR(_client->getSocket(), &_writefds);
        ::send(_client->getSocket(), "~$ ", 3, 0);
    }
    if (not _client->_res_queue.empty()) {
        FD_SET(_client->getSocket(), &_writefds);
    }
}

void tetriq::Rcon::handleClient()
{
    if (_client == nullptr) {
        return;
    }
    if (FD_ISSET(_client->getSocket(), &_readfds)) {
        char buffer[1024] = {0};
        const long int valread = ::recv(_client->getSocket(), buffer, 1024, 0);
        if (valread <= 0) {
            ::close(_client->getSocket());
            _client = nullptr;
            RCONLOG(INFO) << "Connection closed" << std::endl;
            return;
        }
        buffer[valread] = '\0';
        if (std::string(buffer).back() == '\n') {
            buffer[valread - 1] = '\0';
        }

        std::string full_command = buffer;
        std::istringstream iss(full_command);
        std::string command;
        std::vector<std::string> arguments;

        parseCmd(iss, command, arguments);
        if (command == "quit") {
            disconnectClient();
            return;
        }
        if (checkAuth(command))
            return;

        _command_handler.executeCommand(command, arguments, _client->_res_queue);
    }
    sendRes();
}

void tetriq::Rcon::listen()
{
    if (_is_running == false) {
        return;
    }
    FD_ZERO(&_readfds);
    FD_SET(_rcon_socket, &_readfds);
    if (_client != nullptr) {
        FD_SET(_client->getSocket(), &_readfds);
    }
    int activity = select(FD_SETSIZE, &_readfds, nullptr, nullptr, &_timeout);
    if (activity < 0) {
        RCONLOG(ERROR) << "Select error" << std::endl;
        return;
    }
    handleNewClient();
    handleClient();
}

bool tetriq::Rcon::init()
{
    if (_config.enabled == false) {
        return false;
    }
    if (not setHost() or not createHost()) {
        return false;
    }
    _is_running = true;
    return true;
}

void tetriq::Rcon::registerCommand(const std::string &commandName,
    void (ServerManager::*func)(const std::vector<std::string> &, std::queue<std::string> &))
{
    _command_handler.registerCommand(commandName,
        std::make_unique<
            std::function<void(const std::vector<std::string> &, std::queue<std::string> &)>>(
            [this, func](const std::vector<std::string> &args, std::queue<std::string> &res_queue) {
                (this->_server_manager.*func)(args, res_queue);
            }));
}

bool tetriq::Rcon::setHost()
{
    if (not _config.enabled) {
        return false;
    }
    _rcon_port = _config.listen_port;
    _rcon_address.sin_family = AF_INET;
    _rcon_address.sin_addr.s_addr = inet_addr(_config.listen_address.c_str());
    _rcon_address.sin_port = htons(_rcon_port);
    RCONLOG(INFO) << "Host set to " << _config.listen_address << ":" << _config.listen_port
                  << std::endl;
    return true;
}

bool tetriq::Rcon::createHost()
{
    if (not _config.enabled) {
        return false;
    }
    _rcon_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_rcon_socket == -1) {
        RCONLOG(ERROR) << "Failed to create socket" << std::endl;
        return false;
    }
    int opt = 1;
    setsockopt(
        _rcon_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT | SO_KEEPALIVE, &opt, sizeof(opt));
    if (bind(_rcon_socket, (struct sockaddr *) &_rcon_address, sizeof(_rcon_address)) < 0) {
        RCONLOG(ERROR) << "Failed to bind socket" << std::endl;
        return false;
    }
    if (::listen(_rcon_socket, 1) < 0) {
        RCONLOG(ERROR) << "Failed to listen" << std::endl;
        return false;
    }
    RCONLOG(INFO) << "Server listening on " << _config.listen_address << ":" << _config.listen_port
                  << std::endl;
    return true;
}
