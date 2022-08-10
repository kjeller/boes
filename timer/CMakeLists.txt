add_executable(timer_exec
    main.c
    timer.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(timer_exec pico_stdlib)

pico_enable_stdio_usb(timer_exec 1)
pico_enable_stdio_uart(timer_exec 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(timer_exec)
