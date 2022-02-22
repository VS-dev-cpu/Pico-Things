add_executable(Pong main.c)

target_link_libraries(Pong pico_stdlib hardware_clocks pico_multicore pico_lcd)

pico_add_extra_outputs(Pong)