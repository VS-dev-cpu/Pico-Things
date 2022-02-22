add_executable(keyboard main.c)

target_link_libraries(keyboard pico_stdlib hardware_clocks pico_multicore pico_lcd)

pico_add_extra_outputs(keyboard)