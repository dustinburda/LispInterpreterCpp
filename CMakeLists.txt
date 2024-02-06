cmake_minimum_required(VERSION 3.17)

set(This MAL)
project(${This})

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${This}
        step3_env.cpp
        Reader.cpp
        Printer.cpp
        Environment.cpp
        step4_if_fn_do.cpp
)