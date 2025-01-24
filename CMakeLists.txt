cmake_minimum_required(VERSION 3.30)
project(ChatApp)

set(CMAKE_CXX_STANDARD 20)

add_executable(ServerApp server.cpp)
add_executable(ClientApp client.cpp)

target_link_libraries(ServerApp ws2_32)
target_link_libraries(ClientApp ws2_32)
