message("CURRENT_DIR:" ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(timer_t timer_t.cpp)
target_link_libraries(timer_t wasi_timer)