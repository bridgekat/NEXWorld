cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_ROOT_PATH ${CMAKE_SOURCE_DIR})
set(BINARY_DIR ${PROJECT_ROOT_PATH}/release/Binaries/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})

add_subdirectory(src/shared)

add_subdirectory(src/client)

add_subdirectory(src/server)

add_subdirectory(src/mainplugin)
