cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project(speedometer)

add_library(data_bus SHARED data_bus.cpp)

add_compile_options(
"-Wall" "-fPIC"
)

add_executable(speed_generator speed_generator.cpp)
add_executable(speed_display speed_display.cpp)

target_link_libraries(data_bus pthread)

target_link_libraries(speed_generator data_bus)
target_link_libraries(speed_display data_bus)
