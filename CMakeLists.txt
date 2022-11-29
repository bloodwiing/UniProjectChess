cmake_minimum_required(VERSION 3.23)
project(CHESS C)

set(CMAKE_C_STANDARD 99)

add_executable(CHESS main.c move.h move.c moveset.h moveset.c piece.c team.c team.h defaults.c defaults.h board.c board.h)
