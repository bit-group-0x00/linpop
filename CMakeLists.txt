cmake_minimum_required(VERSION 3.14)
project(linpop C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
include_directories(${GTK2_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS})

add_definitions(${GTK2_CFLAGS_OTHER})

set(CMAKE_C_STANDARD 99)

add_executable(linpop_client src/NET_client.c src/NET_socket.c include/NET_client.h include/NET_macro.h include/NET_socket.h src/UTIL_cJSON.c include/UTIL_cJSON.h src/UI_login.c src/UI_homepage.c src/UI_personalInfo.c src/UI_friendList.c src/UI_createGroup.c src/UI_addFriend.c src/UI_chatView.c src/UI_history.c include/UI_login.h src/UI_dialog.c)
add_executable(linpop_server src/NET_server.c src/NET_socket.c include/NET_server.h include/NET_macro.h include/NET_socket.h src/UTIL_cJSON.c include/UTIL_cJSON.h)

find_package(Threads REQUIRED)
target_link_libraries(linpop_client Threads::Threads -lm)
target_link_libraries(linpop_client ${GTK2_LIBRARIES})

target_link_libraries(linpop_server Threads::Threads -lm)
target_link_libraries(linpop_server ${GTK2_LIBRARIES})