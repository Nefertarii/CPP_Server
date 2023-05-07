set(LoggerSinkSRC 
    consolessink.cpp
    filesink.cpp
)

message("Process:" ${CMAKE_CURRENT_SOURCE_DIR})

add_library(wasi_logger_sink STATIC ${LoggerSinkSRC})