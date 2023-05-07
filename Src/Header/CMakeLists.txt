set(HeaderSRC 
    filehandler.cpp
    filestat.cpp
    latch.cpp
    thread.cpp
)

message("Process:" ${CMAKE_CURRENT_SOURCE_DIR})

add_library(wasi_header STATIC ${HeaderSRC})