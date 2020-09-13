cmake_minimum_required(VERSION 3.17)
project(LPTestTask C)

set(CMAKE_C_STANDARD 11)

add_executable(LPTestTask src/str_utils.c src/str_utils.h src/main.c)