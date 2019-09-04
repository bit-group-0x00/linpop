cmake_minimum_required(VERSION 3.14)
project(linpop C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

set(CMAKE_C_STANDARD 99)

add_executable(linpop_client
        src/DATABASE_mysql.c src/DATABASE_groupMessage.c src/DATABASE_groupUser.c src/DATABASE_group.c src/DATABASE_message.c src/DATABASE_friends.c src/DATABASE_user.c src/UTIL_cJSON.c
        src/NET_socket.c src/NET_client.c
        include/DATABASE_mysql.h include/DATABASE_groupMessage.h include/DATABASE_groupUser.h include/DATABASE_group.h include/DATABASE_message.h include/DATABASE_friends.h include/DATABASE_user.h
        include/NET_socket.h include/NET_macro.h include/NET_client.h
        src/UI_history.c)
add_executable(linpop_server
        src/DATABASE_mysql.c src/DATABASE_groupMessage.c src/DATABASE_groupUser.c src/DATABASE_group.c src/DATABASE_message.c src/DATABASE_friends.c src/DATABASE_user.c src/UTIL_cJSON.c
        src/NET_socket.c src/NET_server.c
        include/DATABASE_mysql.h include/DATABASE_groupMessage.h include/DATABASE_groupUser.h include/DATABASE_group.h include/DATABASE_message.h include/DATABASE_friends.h include/DATABASE_user.h
        include/NET_socket.h include/NET_macro.h include/NET_server.h
        )
add_executable(linpop_UItest
        src/UI_test_dependence.c
        include/NET_client.h
        include/NET_macro.h
        include/UI_interface.h
        src/UTIL_cJSON.c
        include/UTIL_cJSON.h
        src/UI_login.c
        src/UI_homepage.c
        src/UI_createGroup.c
        src/UI_addFriend.c
        src/UI_chatView.c
        src/UI_history.c
        src/groupChatView.c
        )

find_package(Threads REQUIRED)

target_link_libraries(linpop_client Threads::Threads -lm)
target_link_libraries(linpop_client ${GTK3_LIBRARIES})

target_link_libraries(linpop_server Threads::Threads -lm)
target_link_libraries(linpop_server ${GTK3_LIBRARIES})

target_link_libraries(linpop_UItest Threads::Threads -lm)
target_link_libraries(linpop_UItest ${GTK3_LIBRARIES})

