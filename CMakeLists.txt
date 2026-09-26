cmake_minimum_required(VERSION 3.10)
project(y2038_bug_solution_guide C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(y2038_demo
    src/main.c
    src/y2038_safe_time.c
)
