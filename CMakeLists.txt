cmake_minimum_required(VERSION 3.26)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 23)

project(preloader C)

include_directories("include")

link_directories("lib"
                 "lib/nethost")

add_library(preloader SHARED src/main.c src/dotnet.c)

target_link_libraries(preloader "libnethost.a")
