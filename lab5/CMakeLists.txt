cmake_minimum_required(VERSION 3.5.1)
project(lab5 C)

set(SRC src/main.c src/Encode.c src/Encode.h src/Decode.c src/Decode.h src/FileStream.c src/FileStream.h src/BinaryTree.c src/BinaryTree.h src/NodesStack.c src/NodesStack.h)

add_executable(${PROJECT_NAME} ${SRC})
