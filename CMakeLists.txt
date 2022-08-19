cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(picopad C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(picopad)
target_sources(picopad PUBLIC ${CMAKE_CURRENT_LIST_DIR}/main.c ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c ${CMAKE_CURRENT_LIST_DIR}/tasks.c ${CMAKE_CURRENT_LIST_DIR}/Analog/Analog.c)
#target_sources(picopad PUBLIC ${CMAKE_CURRENT_LIST_DIR}/main.c ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c ${CMAKE_CURRENT_LIST_DIR}/tasks.c)
target_include_directories(picopad PUBLIC ${CMAKE_CURRENT_LIST_DIR})

pico_add_extra_outputs(picopad)
target_link_libraries(picopad pico_stdlib hardware_adc tinyusb_device tinyusb_board pico_bootrom pico_bootsel_via_double_reset)