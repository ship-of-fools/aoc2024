cmake_minimum_required(VERSION 3.21)
cmake_policy(VERSION 3.21)

set(CMAKE_C_STANDARD 11)

set(name "Day15")

project(${name} C)

add_executable(${name} "main.c")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static -O3 -Wall -march=native -mtune=native")
set_target_properties(${name} PROPERTIES LINK_SEARCH_START_STATIC ON)
set_target_properties(${name} PROPERTIES LINK_SEARCH_END_STATIC ON)
target_link_options(${name} PRIVATE "-static-libgcc")

target_link_libraries(${name} m)