cmake_minimum_required(VERSION 3.22)

# Mandelbrot project definition
project(mandelbrot VERSION 0.0.1 DESCRIPTION "mandelbrot" LANGUAGES C)
include(FetchContent)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add subprojects
add_subdirectory(libfractal)

add_executable(mandelbrot "mandelbrot.c")
target_include_directories(mandelbrot PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(mandelbrot PUBLIC "libfractal")
