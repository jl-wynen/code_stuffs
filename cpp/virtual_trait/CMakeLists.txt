cmake_minimum_required(VERSION 3.17)
project(deferred_dispatch)

set(CMAKE_CXX_STANDARD 17)

add_executable(deferred_dispatch main.cpp variable.hpp variable.cpp dataset.cpp dataset.hpp get_unit.hpp)