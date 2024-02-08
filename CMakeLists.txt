
cmake_minimum_required(VERSION 3.0.0)
project(unirail_simulation VERSION 0.1.0)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} include/unirail_simulation)

add_library(logic src/logic.c)
add_executable(simulate src/simulate.c)

target_link_libraries(simulate PRIVATE logic SDL2::SDL2)
