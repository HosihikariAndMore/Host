cmake_minimum_required(VERSION 3.28.1)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_COMPILER clang)

project(preload C)

include_directories(include)

link_directories(lib/nethost)

add_library(preload SHARED src/main.c src/dotnet.c)

target_link_libraries(preload libnethost.a libstdc++.so)
