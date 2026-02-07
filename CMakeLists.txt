cmake_minimum_required(VERSION 3.20)
project(echo_server)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "Only support 'Linux' system, current system is '${CMAKE_SYSTEM_NAME}'")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME}
        main.cpp
        tiny_reactor/include/Handle.h
        tiny_reactor/include/Reactor.h
        tiny_reactor/source/Reactor.cpp
        tiny_reactor/include/ReactorImpl.h
        tiny_reactor/source/ReactorImpl.cpp
        tiny_reactor/include/EventDemultiplexer.h
        tiny_reactor/source/EventDemultiplexer.cpp
        tiny_reactor/include/EventHandler.h
        tiny_reactor/source/EventHandler.cpp
)

target_include_directories(${TARGET_NAME} PRIVATE
        tiny_reactor/include
)

target_link_libraries(${TARGET_NAME} PRIVATE
        pthread
)
