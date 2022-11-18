cmake_minimum_required(VERSION 3.23)
project(sfml)

set(CMAKE_CXX_STANDARD 23)

add_executable(sfml main.cpp)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(sfml sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
