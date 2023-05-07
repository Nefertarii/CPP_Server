set(EventLoopSRC 
    channel.cpp
    eventloop.cpp
    eventloopthread.cpp
    eventloopthreadpool.cpp
    poller.cpp
    timerqueue.cpp
)

message("Process:" ${CMAKE_CURRENT_SOURCE_DIR})

add_library(wasi_eventloop STATIC ${EventLoopSRC})