cmake_minimum_required (VERSION 2.6)
project(tidieu)
add_executable(tidieu src/judy64d.c src/double_link.c src/lru.c src/cherly.c src/tidieu.c)
