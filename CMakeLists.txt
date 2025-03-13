# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

# Define the board (pico_w for Wi-Fi, pico for non-Wi-Fi)
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Initialize the project
project(MP3 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(MP3 MP3.c inc/ssd1306_i2c.c)

# Set program name and version
pico_set_program_name(MP3 "MP3")
pico_set_program_version(MP3 "0.1")

# Enable/disable output over UART/USB
pico_enable_stdio_uart(MP3 0)
pico_enable_stdio_usb(MP3 1)


# Add the standard library to the build
target_link_libraries(MP3
    pico_stdlib
    hardware_adc
    hardware_pwm
    hardware_gpio
    hardware_i2c
    hardware_clocks
    hardware_pio  
)

# Add the standard include files to the build
target_include_directories(MP3 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(MP3
    hardware_i2c
    hardware_pwm
    hardware_adc
    hardware_clocks
    pico_stdlib
    hardware_gpio
    hardware_pio  
)


# Generate additional output files (UF2, HEX, etc.)
pico_add_extra_outputs(MP3)