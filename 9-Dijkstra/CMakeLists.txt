cmake_minimum_required(VERSION 3.5.1)
project(lab9 C)

set(SRC src/main.c src/Graph.h src/Graph.c src/PriorityQueue.h src/PriorityQueue.c src/MemManager.h src/MemManager.c)
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

add_executable(${PROJECT_NAME} ${SRC})
