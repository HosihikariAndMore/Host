cmake_minimum_required(VERSION 3.26)
project(preloader C)

set(CMAKE_C_STANDARD 23)

include_directories("include")

link_directories(
        "lib"
        "lib/nethost"
)

add_library(preloader SHARED src/main.c)

target_link_libraries(preloader "libnethost.a")
