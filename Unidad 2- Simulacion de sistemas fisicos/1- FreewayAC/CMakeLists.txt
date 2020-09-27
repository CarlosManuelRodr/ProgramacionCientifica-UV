cmake_minimum_required(VERSION 3.14)
project(FreewayAC)

set(CMAKE_CXX_STANDARD 14)

include_directories(CLI)
include_directories(FreewayAC)

add_executable(FreewayAC
        CLI/main.cpp
        CLI/optionparser.h
        FreewayAC/Auxiliar.cpp
        FreewayAC/Auxiliar.h
        FreewayAC/BmpWriter.cpp
        FreewayAC/BmpWriter.h
        FreewayAC/CellularAutomata.cpp
        FreewayAC/CellularAutomata.h)
