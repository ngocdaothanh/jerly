cmake_minimum_required(VERSION 2.6)
project(jerly)

find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

add_library(jerly SHARED src/main/c/judy64d.c src/main/c/double_link.c src/main/c/lru.c src/main/c/cherly.c src/main/c/Jerly.c)
