cmake_minimum_required(VERSION 3.20)
project(echo_server)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "Only support 'Linux' system, current system is '${CMAKE_SYSTEM_NAME}'")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME}
        ${ROOT_DIR}/echo_server.cpp
)

target_include_directories(${TARGET_NAME} PRIVATE
        ${ROOT_DIR}/inc
)

target_link_libraries(${TARGET_NAME} PRIVATE
        pthread
)
