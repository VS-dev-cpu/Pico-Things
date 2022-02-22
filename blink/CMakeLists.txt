add_executable(blink main.c)

target_link_libraries(blink pico_stdlib hardware_vreg)

pico_add_extra_outputs(blink)