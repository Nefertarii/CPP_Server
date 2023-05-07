cmake_minimum_required(VERSION 3.10)

project(WebServer VERSION 2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

message("Process:" ${PROJECT_SOURCE_DIR})
#configure_file(webserverconfig.h)
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(Src/Eventloop)
add_subdirectory(Src/Header)
add_subdirectory(Src/Logger)
add_subdirectory(Src/Net)
add_subdirectory(Src/Timer)
add_subdirectory(Src/Web/Http)
add_subdirectory(Test)

link_libraries(
    pthread
    wasi_eventloop 
    wasi_header
    wasi_logger
    wasi_logger_sink
    wasi_net
    wasi_timer
    wasi_web_http
)

add_executable(webserver webserver.cpp)

target_link_libraries(webserver     
    wasi_eventloop 
    wasi_header
    wasi_logger
    wasi_logger_sink
    wasi_net
    wasi_timer
    wasi_web_http
)

set(SERVER_SETTING 
"listen_addr: 0.0.0.0
listen_port: 8000
reuse_port: true
html_resource_directory: ${PROJECT_SOURCE_DIR}/HTML
fail_repeat_num: 10
#The time unit is ms
fail_repeat_time: 50
server_thread_num: 4
server_name: Wasi's Server"
)

file(WRITE build/server_setting.txt "${SERVER_SETTING}")