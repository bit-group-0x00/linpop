cmake_minimum_required(VERSION 3.14)
project(linpop C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
include_directories(${GTK2_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS})

add_definitions(${GTK2_CFLAGS_OTHER})

set(CMAKE_C_STANDARD 99)

add_executable(linpop_client src/NET_client.c src/NET_socket.c include/NET_client.h include/NET_macro.h include/NET_socket.h src/util/cJSON.c src/util/cJSON.h)
add_executable(linpop_server src/NET_server.c src/NET_socket.c include/NET_server.h include/NET_macro.h include/NET_socket.h src/util/cJSON.c src/util/cJSON.h)

# this is a executable module for testing database
add_executable(test src/testMain.c include/DATABASE_user.h include/DATABASE_friends.h include/DATABASE_message.h
        include/DATABASE_group.h include/DATABASE_groupUser.h include/DATABASE_groupMessage.h include/DATABASE_mysql.h)

find_package(Threads REQUIRED)
target_link_libraries(linpop_client Threads::Threads -lm)
target_link_libraries(linpop_client ${GTK2_LIBRARIES})

target_link_libraries(linpop_server Threads::Threads -lm)
target_link_libraries(linpop_server ${GTK2_LIBRARIES})