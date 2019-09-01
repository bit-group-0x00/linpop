cmake_minimum_required(VERSION 3.14)
project(linpop C)

find_package(PkgConfig REQUIRED)

set(CMAKE_C_STANDARD 99)

# this is a executable module for testing database
add_executable(test src/testMain.c include/DATABASE_user.h include/DATABASE_friends.h include/DATABASE_message.h
        include/DATABASE_group.h include/DATABASE_groupUser.h include/DATABASE_groupMessage.h include/DATABASE_mysql.h
        src/DATABASE_user.c)
target_link_libraries(test -lmysqlclient)