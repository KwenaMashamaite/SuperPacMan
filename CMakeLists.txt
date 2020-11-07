cmake_minimum_required(VERSION 3.17)
project(SuperPacMan LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Super Pac-man clone using C++17 and above")

set(CMAKE_CXX_STANDARD 17)

#Build game
add_subdirectory(src)