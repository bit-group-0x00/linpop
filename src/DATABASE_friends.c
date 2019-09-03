//
// Created by junqi on 2019/9/1.
//
#include "../include/DATABASE_friends.h"

Status isFriends(int userId1, int userId2, MYSQL *connection) {
    if (connection == NULL) {
        perror("IS FRIENDS: CONNECTION NULL ERROR\n");
        return -1;
    }
    if (userId1 == userId2) {
        printf("IS FRIENDS: SAME ID ERROR\n");
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
        perror("IS FRIEND: QUERY ERROR\n");
    } else {
        res = mysql_store_result(connection);
        if (res) {
            row = mysql_fetch_row(res);
            if (row) {
                return row[0][0] - '0' == 0 ? 0 : 1;
            }
        } else {
            perror("IS FRIEND: RES NULL ERROR\n");
        }
    }
    return -1;
}

Status insertFriends(int userId1, int userId2, MYSQL *connection) {
    if (connection == NULL) {
        perror("INSERT FRIENDS: CONNECTION NULL ERROR\n");
        return -1;
    }
    if (userId1 == userId2) {
        perror("INSERT FRIENDS: SAME ID ERROR\n");
        return -1;
    }
    mysql_query(connection,"SET NAMES utf8");

    char insertSql[SQL_LENGTH_MAX];
    memset(insertSql, '\0', sizeof(insertSql));
    if (userId1 < userId2) {
        if (isFriends(userId1, userId2, connection) == 1) {
            perror("INSERT FRIENDS: ALREADY FRIENDS ERROR\n");
            return -1;
        }
        sprintf(insertSql, "INSERT INTO linpop.friends(friUserId1, friUserId2)\n"
                           "VALUES(%d,%d);", userId1, userId2);
    } else {
        if (isFriends(userId2, userId1, connection) == 1) {
            perror("INSERT FRIENDS: ALREADY FRIENDS ERROR\n");
            return -1;
        }
        sprintf(insertSql, "INSERT INTO linpop.friends(friUserId1, friUserId2)\n"
                           "VALUES(%d,%d);", userId2, userId1);
    }

    if (mysql_real_query(connection, insertSql, strlen(insertSql))) {
        perror("INSERT FRIENDS: QUERY ERROR\n");
        return -1;
    } else {
        updateUserFriendNum(userId1, connection, INCREASE);
        updateUserFriendNum(userId2, connection, INCREASE);
        return 1;
    }
}

FriendList getFriList(int userId, MYSQL *connection) {
    FriendList friendList;
    friendList.friId = NULL;
    friendList.userId = userId;
    friendList.friendsNum = 0;
    if (connection == NULL) {
        perror("GET FRIEND LIST: CONNECTION NULL ERROR\n");
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
        perror("GET FRIEND LIST: QUERY ERROR\n");
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
            perror("GET FRIEND LIST: RES NULL ERROR\n");
        }
    }
    return friendList;
}

Status deleteFriends(int userId1, int userId2, MYSQL *connection) {
    if (connection == NULL) {
        perror("DELETE FRIENDS: CONNECTION NULL ERROR\n");
        return -1;
    }
    if (userId1 == userId2) {
        perror("DELETE FRIENDS: SAME ID ERROR\n");
        return -1;
    }
    mysql_query(connection,"SET NAMES utf8");

    char insertSql[SQL_LENGTH_MAX];
    memset(insertSql, '\0', sizeof(insertSql));
    if (userId1 < userId2) {
        if (isFriends(userId1, userId2, connection) == 1) {
            sprintf(insertSql, "DELETE FROM linpop.friends\n"
                               "WHERE friUserId1=%d and friUserId2=%d;", userId1, userId2);
        } else {
            perror("DELETE FRIENDS: NO SUCH FRIENDS ERROR\n");
            return -1;
        }
    } else {
        if (isFriends(userId2, userId1, connection) == 1) {
            sprintf(insertSql, "DELETE FROM linpop.friends\n"
                               "WHERE friUserId1=%d and friUserId2=%d;", userId2, userId1);
        } else {
            perror("DELETE FRIENDS: NO SUCH FRIENDS ERROR\n");
            return -1;
        }
    }

    if (mysql_real_query(connection, insertSql, strlen(insertSql))) {
        perror("DELETE FRIENDS: QUERY ERROR\n");
        return -1;
    } else {
        updateUserFriendNum(userId1, connection, DECREASE);
        updateUserFriendNum(userId2, connection, DECREASE);
        return 1;
    }
}

void freeFriList(FriendList friendList) {
    if(friendList.friId != NULL) {
        free(friendList.friId);
    }
}