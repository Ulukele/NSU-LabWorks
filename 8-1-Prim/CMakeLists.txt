cmake_minimum_required(VERSION 3.5.1)
project(lab8-1 C)

set(SRC src/main.c src/Graph.h src/Graph.c src/PriorityQueue.h src/PriorityQueue.c)
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

add_executable(${PROJECT_NAME} ${SRC})
