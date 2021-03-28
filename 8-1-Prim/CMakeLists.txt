cmake_minimum_required(VERSION 3.5.1)
project(lab8-1 C)

set(SRC src/main.c src/Graph.h src/Graph.c src/PriorityQueue.h src/PriorityQueue.c)

add_executable(${PROJECT_NAME} ${SRC})
