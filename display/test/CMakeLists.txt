add_executable(test main.c)

target_link_libraries(test pico_stdlib hardware_clocks pico_multicore pico_lcd)

pico_add_extra_outputs(test)