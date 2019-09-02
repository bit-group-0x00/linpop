//
// Created by junqi on 2019/9/1.
//
#include "../include/DATABASE_user.h"
#include "../include/DATABASE_friends.h"
#include <string.h>
#include <stdio.h>
#define SQL_LENGTH_MAX 1000

Status isFriends(int userId1, int userId2, MYSQL *connection) {
    if (connection == NULL) {
        printf("connection is NULL");
        return -1;
    }
    if (userId1 == userId2) {
        printf("the same ID error");
        return -1;
    }
    mysql_query(connection,"SET NAMES utf8");

    char querySql[SQL_LENGTH_MAX];
    memset(querySql, '\0', sizeof(querySql));
    if (userId1 < userId2) {
        sprintf(querySql, "SELECT COUNT(friId) AS amount\n"
                          "FROM linpop.friends\n"
                          "WHERE friUserId1=%d and friUserId2=%d;", userId1, userId2);
    } else {
        sprintf(querySql, "SELECT COUNT(friId) AS amount\n"
                          "FROM linpop.friends\n"
                          "WHERE friUserId1=%d and friUserId2=%d;", userId2, userId1);
    }

    MYSQL_RES* res;
    MYSQL_ROW row;

    if (mysql_real_query(connection, querySql, strlen(querySql))) {
        printf("query is friends failed\n");
    } else {
        res = mysql_store_result(connection);
        if (res) {
            row = mysql_fetch_row(res);
            if (row) {
                printf("query is friends success\n");
                return row[0][0] - '0' == 0 ? false : true;
            }
        } else {
            printf("query is friends: res is NULL\n");
        }
    }
    return -1;
}

Status insertFriends(int userId1, int userId2, MYSQL *connection) {
    if (connection == NULL) {
        printf("connection is NULL");
        return false;
    }
    if (userId1 == userId2) {
        printf("same user id error");
        return false;
    }
    mysql_query(connection,"SET NAMES utf8");

    char insertSql[SQL_LENGTH_MAX];
    memset(insertSql, '\0', sizeof(insertSql));
    if (userId1 < userId2) {
        if (isFriends(userId1, userId2, connection) == true) {
            printf("already friends error");
            return false;
        }
        sprintf(insertSql, "INSERT INTO linpop.friends(friUserId1, friUserId2)\n"
                           "VALUES(%d,%d);", userId1, userId2);
    } else {
        if (isFriends(userId2, userId1, connection) == true) {
            printf("already friends error");
            return false;
        }
        sprintf(insertSql, "INSERT INTO linpop.friends(friUserId1, friUserId2)\n"
                           "VALUES(%d,%d);", userId2, userId1);
    }

    if (mysql_real_query(connection, insertSql, strlen(insertSql))) {
        printf("insert friends fail\n");
        return false;
    } else {
        printf("insert success\n");
        updateUserFriendNum(userId1, connection, INCREASE);
        updateUserFriendNum(userId2, connection, INCREASE);
        return true;
    }
}

FriendList getFriList(int userId, MYSQL *connection) {
    FriendList friendList;
    friendList.friId = NULL;
    friendList.userId = userId;
    friendList.friendsNum = 0;
    if (connection == NULL) {
        printf("connection is NULL");
        return friendList;
    }
    mysql_query(connection,"SET NAMES utf8");
    char querySql[SQL_LENGTH_MAX];
    memset(querySql, 0, sizeof(querySql));
    sprintf(querySql, "SELECT *\n"
                      "FROM linpop.friends\n"
                      "WHERE friUserId1=%d OR friUserId2=%d;", userId, userId);

    MYSQL_RES* res;
    MYSQL_ROW row;
    if (mysql_real_query(connection, querySql, strlen(querySql))) {
        printf("query friends list failed\n");
    } else {
        res = mysql_store_result(connection);
        if (res) {
            int rowNum = mysql_num_rows(res);
            friendList.friendsNum = rowNum;
            friendList.friId = (int *) malloc(sizeof(int) * rowNum);
            row = mysql_fetch_row(res);
            int index = 0;
            while (row) {
                int id1 = atoi(row[1]);
                int id2 = atoi(row[2]);
                if (id1 == userId) {
                    *(friendList.friId + index) = id2;
                } else {
                    *(friendList.friId + index) = id1;
                }
                index++;
                row = mysql_fetch_row(res);
            }
        } else {
            printf("query user info: res is NULL");
        }
    }
    return friendList;
}

Status deleteFriends(int userId1, int userId2, MYSQL *connection) {
    if (connection == NULL) {
        printf("connection is NULL");
        return false;
    }
    if (userId1 == userId2) {
        printf("same user id error");
        return false;
    }
    mysql_query(connection,"SET NAMES utf8");

    char insertSql[SQL_LENGTH_MAX];
    memset(insertSql, '\0', sizeof(insertSql));
    if (userId1 < userId2) {
        if (isFriends(userId1, userId2, connection) == true) {
            sprintf(insertSql, "DELETE FROM linpop.friends\n"
                               "WHERE friUserId1=%d and friUserId2=%d;", userId1, userId2);
        } else {
            printf("user1 and user2 are not friends error");
            return false;
        }
    } else {
        if (isFriends(userId2, userId1, connection) == true) {
            sprintf(insertSql, "DELETE FROM linpop.friends\n"
                               "WHERE friUserId1=%d and friUserId2=%d;", userId2, userId1);
        } else {
            printf("user1 and user2 are not friends error");
            return false;
        }
    }

    if (mysql_real_query(connection, insertSql, strlen(insertSql))) {
        printf("delete friends fail\n");
        return false;
    } else {
        printf("delete success\n");
        updateUserFriendNum(userId1, connection, DECREASE);
        updateUserFriendNum(userId2, connection, DECREASE);
        return true;
    }
}

void freeFriList(FriendList friendList) {
    if(friendList.friId != NULL) {
        free(friendList.friId);
    }
}