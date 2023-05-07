set(LoggerSRC 
    appender.cpp
    formatter.cpp
    header.cpp
    logger.cpp
)

message("Process:" ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Sink)

add_library(wasi_logger STATIC ${LoggerSRC})