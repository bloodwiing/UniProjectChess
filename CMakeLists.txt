cmake_minimum_required(VERSION 3.23)
project(CHESS C)

set(CMAKE_C_STANDARD 99)

add_executable(CHESS main.c model/move.h model/move.c model/moveset.h model/moveset.c model/piece.c model/team.c model/team.h defaults.c defaults.h model/scenario.c model/scenario.h model/spawn.c model/spawn.h model/defs.h ui/board.c ui/board.h ui/con_lib.c ui/con_lib.h settings/settings.c settings/settings.h utils/files.c utils/files.h)
