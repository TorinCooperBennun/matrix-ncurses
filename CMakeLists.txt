cmake_minimum_required(VERSION 3.6)
project(MATRIX)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses)

set(CMAKE_C_FLAGS "-g -Wall -std=c99 -D_POSIX_C_SOURCE=199309L")

add_executable(matrix main.c matrix.c)
target_link_libraries(matrix ${CURSES_LIBRARIES})
