cmake_minimum_required(VERSION 3.23)
project(CHESS C)

set(CMAKE_C_STANDARD 99)

FILE(GLOB CHESS_SOURCES *.c abstract/*.c abstract/*.h engine/*.c model/*.c model/move/*.c settings/*.c ui/*.c ui/component/*.c ui/component/responsive/*.c ui/generic/*.c ui/view/*.c utils/*.c)
add_executable(CHESS ${CHESS_SOURCES})

target_include_directories(CHESS PRIVATE .)
target_link_libraries(CHESS m)
