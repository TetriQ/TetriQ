cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

project(tetriq)

find_package(SFML 2.6 COMPONENTS system window graphics network audio REQUIRED)
find_package(ENet REQUIRED)
find_package(Curses REQUIRED)
find_package(libconfig REQUIRED)

include_directories(
    server/include
    client/include
    common/include
    ${ENET_INCLUDE_DIRS}
    ${CURSES_INCLUDE_DIRS}
    ${LIBCONFIG_INCLUDE_DIRS}
)

#
# SERVER
#

file(GLOB_RECURSE SERVER_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/server/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/src/*.cpp
)

add_executable(tetriq_server ${SERVER_SRCS})

target_link_libraries(tetriq_server
    ${ENET_LIBRARIES}
    ${CURSES_LIBRARIES}
    ${LIBCONFIG_LIBRARIES}
)

add_custom_target(tetriq-server
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target tetriq_server
    COMMENT "Build the server"
    VERBATIM
)

#
# CLIENT
#

file(GLOB_RECURSE CLIENT_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/client/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/src/*.cpp
)

add_executable(tetriq_client ${CLIENT_SRCS})

target_link_libraries(tetriq_client
    ${SFML_LIBRARIES}
    ${ENET_LIBRARIES}
    ${CURSES_LIBRARIES}
    ${LIBCONFIG_LIBRARIES}
)

add_custom_target(tetriq-client
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target tetriq_client
    COMMENT "Build the client"
    VERBATIM
)