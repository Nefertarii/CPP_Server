set(WebHttpSRC
    httpcontext.cpp
    httphead.cpp
    httpprocess.cpp
    httprequest.cpp
    httprespone.cpp
    httpserver.cpp
)

message("Process:" ${CMAKE_CURRENT_SOURCE_DIR})

add_library(wasi_web_http STATIC ${WebHttpSRC})