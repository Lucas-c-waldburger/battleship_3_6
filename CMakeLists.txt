cmake_minimum_required(VERSION 3.24)
project(battleship_3_6)

set(CMAKE_CXX_STANDARD 20)

add_executable(battleship_3_6 main.cpp utils/battleship_logic.h utils/battleship_logic.cpp)
