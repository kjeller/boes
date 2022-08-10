cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(boes)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

set(st_sources
    s_task/src/s_task.c
    s_task/src/s_mutex.c
    s_task/src/s_event.c
    s_task/src/s_event_embed.c
    s_task/src/s_chan.c
    s_task/src/s_chan_embed.c
    s_task/src/s_timer_fast.c
    s_task/src/s_timer_small.c
    s_task/src/s_list.c
    s_task/src/s_rbtree.c)

add_executable(timer_exec
    timer/main.cpp
    timer/timer.cpp
    input/input_event_buffer.cpp
    input/input_system.cpp
    input/reg_gpio_key.cpp
    Hal/hal_gpio_key.cpp
    Hal/hal_RGBMatrix_device.cpp
    ModuleDrivers/driver_key.cpp
    ModuleDrivers/driver_buzzer.cpp
    ModuleDrivers/driver_ds3231.cpp
    ModuleDrivers/driver_adc.cpp
    ModuleDrivers/driver_RGBMatrix.cpp
    device/RGBMatrix_device.cpp
    font/font.cpp
    ${st_sources}
)

target_include_directories(timer_exec PRIVATE 
                        s_task/include 
                        Hal
                        ModuleDrivers
                        input
                        device
                        config
                        font)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(timer_exec pico_stdlib hardware_i2c hardware_adc)

pico_enable_stdio_usb(timer_exec 1)
pico_enable_stdio_uart(timer_exec 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(timer_exec)
