set(TimerSRC 
    clock.cpp
    timer.cpp
    timestamp.cpp
)

message("Process:" ${CMAKE_CURRENT_SOURCE_DIR})

add_library(wasi_timer STATIC ${TimerSRC})