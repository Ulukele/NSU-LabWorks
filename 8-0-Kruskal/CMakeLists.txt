cmake_minimum_required(VERSION 3.5.1)
project(lab8-0 C)

set(SRC src/main.c src/DisjointSetUnion.h src/DisjointSetUnion.c src/Graph.h src/Graph.c)

add_executable(${PROJECT_NAME} ${SRC})
