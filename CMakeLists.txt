cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_7seg C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pico_7seg)

target_sources(pico_7seg PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        )

target_include_directories(pico_7seg PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

pico_add_extra_outputs(pico_7seg)

target_link_libraries(pico_7seg 
    pico_stdlib
    hardware_gpio
)