cmake_minimum_required(VERSION 3.16)
project(lab5 C)

set(CMAKE_C_STANDARD 99)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

add_executable(lab5 main.c Encode.c Encode.h Decode.c Decode.h)
