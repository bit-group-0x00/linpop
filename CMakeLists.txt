cmake_minimum_required(VERSION 3.14)
project(linpop C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
include_directories(${GTK2_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS})

add_definitions(${GTK2_CFLAGS_OTHER})

set(CMAKE_C_STANDARD 99)

add_executable(linpop_client
        src/DATABASE_mysql.c src/DATABASE_groupMessage.c src/DATABASE_groupUser.c src/DATABASE_group.c src/DATABASE_message.c src/DATABASE_friends.c src/DATABASE_user.c src/UTIL_cJSON.c
        src/NET_socket.c src/NET_client.c
        include/DATABASE_mysql.h include/DATABASE_groupMessage.h include/DATABASE_groupUser.h include/DATABASE_group.h include/DATABASE_message.h include/DATABASE_friends.h include/DATABASE_user.h
        include/NET_socket.h include/NET_macro.h include/NET_client.h
        )

add_executable(linpop_server
        src/DATABASE_mysql.c src/DATABASE_groupMessage.c src/DATABASE_groupUser.c src/DATABASE_group.c src/DATABASE_message.c src/DATABASE_friends.c src/DATABASE_user.c src/UTIL_cJSON.c
        src/NET_socket.c src/NET_server.c
        include/DATABASE_mysql.h include/DATABASE_groupMessage.h include/DATABASE_groupUser.h include/DATABASE_group.h include/DATABASE_message.h include/DATABASE_friends.h include/DATABASE_user.h
        include/NET_socket.h include/NET_macro.h include/NET_server.h
        )

target_link_libraries(linpop_client ${GTK2_LIBRARIES} -lpthread -lm -lmysqlclient)

target_link_libraries(linpop_server ${GTK2_LIBRARIES} -lpthread -lm -lmysqlclient)
