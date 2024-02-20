cmake_minimum_required(VERSION 3.0.0)
project(unirail_simulation VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_compile_definitions(DEBUG)
endif()

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} include/unirail_simulation)

add_library(logic src/logic.c)
add_library(file src/file.c)
add_library(plot src/plot.c)
add_library(utils src/utils.c)
add_executable(simulate src/simulate.c)

target_link_libraries(logic utils m)
target_link_libraries(file logic utils)
target_link_libraries(plot utils logic SDL2::SDL2)
target_link_libraries(
  simulate
  logic
  file
  plot
  utils
  SDL2::SDL2)

