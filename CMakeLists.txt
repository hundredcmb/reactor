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
        # 事件循环模块(EventLoop)
        tiny_reactor/include/Reactor.h
        tiny_reactor/source/Reactor.cpp
        tiny_reactor/include/ReactorImpl.h
        tiny_reactor/source/ReactorImpl.cpp
        tiny_reactor/include/EventDemultiplexer.h
        tiny_reactor/include/EpollDemultiplexer.h
        tiny_reactor/source/EpollDemultiplexer.cpp

        # 事件处理模块(Channel)
        tiny_reactor/include/Handle.h
        tiny_reactor/include/EventHandler.h
        tiny_reactor/include/ListenHandler.h
        tiny_reactor/source/ListenHandler.cpp
        tiny_reactor/include/SockHandler.h
        tiny_reactor/source/SockHandler.cpp

        # 其他
        main.cpp
        tiny_reactor/include/noncopyable.h
        tiny_reactor/include/V4Address.h
)

target_include_directories(${TARGET_NAME} PRIVATE
        tiny_reactor/include
)

target_link_libraries(${TARGET_NAME} PRIVATE
        pthread
)
