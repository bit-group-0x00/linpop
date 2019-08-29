cmake_minimum_required(VERSION 3.14)
project(linpop C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
include_directories(${GTK2_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS})

add_definitions(${GTK2_CFLAGS_OTHER})

set(CMAKE_C_STANDARD 99)

add_executable(linpop main.c cmake-build-debug/main.c Test.c)

find_package(Threads REQUIRED)
target_link_libraries(linpop Threads::Threads)
target_link_libraries(linpop ${GTK2_LIBRARIES})