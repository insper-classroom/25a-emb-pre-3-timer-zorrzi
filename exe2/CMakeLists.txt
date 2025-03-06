# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location (note this can come from
# environment, CMake cache etc) set(PICO_SDK_PATH
# "/home/mathew/src/pico/pico-sdk")

project(Exe2 C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(exe2 main.c)

pico_set_program_name(exe2 "exe2")
pico_set_program_version(exe2 "0.1")

pico_enable_stdio_uart(exe2 1)
pico_enable_stdio_usb(exe2 0)

# Add the standard library to the build
target_link_libraries(exe2 pico_stdlib)

# Add any user requested libraries
target_link_libraries(exe2 hardware_clocks)

pico_add_extra_outputs(exe2)
