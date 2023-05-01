cmake_minimum_required(VERSION 3.22)
project(MathParser)

set(CMAKE_CXX_STANDARD 20)

add_executable(MathParser main.cpp MathParser.hpp Operations.hpp Fraction.hpp)
