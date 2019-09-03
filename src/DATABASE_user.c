//
// Created by junqi on 2019/8/31.
//

#include "../include/DATABASE_user.h"

void mallocUser(User *user) {
    if (user == NULL) {
        return;
    }
    user->userAvatar = (char *) malloc(sizeof(char) * 100);
    memset(user->userAvatar, 0, sizeof(char) * 100);
    user->userSignature = (char *) malloc(sizeof(char) * 100);
    memset(user->userSignature, 0, sizeof(char) * 100);
    user->userPassword = (char *) malloc(sizeof(char) * 30);
    memset(user->userPassword, 0, sizeof(char) * 30);
    user->userNickName = (char *) malloc(sizeof(char) * 30);
    memset(user->userNickName, 0, sizeof(char) * 30);
}

void freeUser(User *user) {
    if (user == NULL) {
        return;
    }
    if(user->userNickName != NULL){
        free(user->userNickName);}
    if(user->userPassword != NULL)
        free(user->userPassword);
    if(user->userSignature != NULL)
        free(user->userSignature);
    if(user->userAvatar != NULL)
        free(user->userAvatar);
    free(user);
}

User *getUserInfoById(int userId, MYSQL *connection) {
    if (connection == NULL) {
        perror("GET USER INFO BY ID: CONNECTION NULL ERROR\n");
        return NULL;
    }
    mysql_query(connection,"SET NAMES utf8");
    char querySql[SQL_LENGTH_MAX];
    memset(querySql, 0, sizeof(querySql));
    sprintf(querySql, "SELECT userId, userFriNum, userNickName, userSignature, userStatus, userAvatar, userIp\n"
                      "FROM linpop.user\n"
                      "WHERE userId=%d;", userId);
    MYSQL_RES* res;
    MYSQL_ROW row;

    if (mysql_real_query(connection, querySql, strlen(querySql))) {
        perror("GET USER INFO BY ID: QUERY ERROR\n");
    } else {
        res = mysql_store_result(connection);
        if (res) {
            row = mysql_fetch_row(res);
            if (row) {
                User *user = (User *) malloc(sizeof(User));
                mallocUser(user);
                user->userId = userId;
                user->userFriNum = atoi(row[1]);
                strcpy(user->userNickName, row[2]);
                strcpy(user->userSignature, row[3]);
                user->userStatus = atoi(row[4]);
                strcpy(user->userAvatar, row[5]);;
                user->userIp = atoi(row[6]);
                return user;
            }
        } else {
            printf("GET USER INFO BY ID: RES NULL ERROR\n");
        }
    }
    return NULL;
}

Status isUserExist(int userId, MYSQL *connection) {
    if (connection == NULL) {
        perror("IS USER EXIST: CONNECTION NULL ERROR\n");
        return -1;
    }
    mysql_query(connection,"SET NAMES utf8");

    char querySql[SQL_LENGTH_MAX];
    memset(querySql, '\0', sizeof(querySql));
    sprintf(querySql, "SELECT COUNT(userId) AS amount\n"
                      "FROM linpop.user\n"
                      "WHERE userId=%d;", userId);

    MYSQL_RES* res;
    MYSQL_ROW row;

    if (mysql_real_query(connection, querySql, strlen(querySql))) {
        perror("IS USER EXIST: QUERY ERROR\n");
    } else {
        res = mysql_store_result(connection);
        if (res) {
            row = mysql_fetch_row(res);
            if (row) {
                return row[0][0] - '0' == 0 ? 0 : 1;
            }
        } else {
            perror("IS USER EXIST: RES NULL ERROR\n");
        }
    }
    return -1;
}

int insertUser(User* user, MYSQL *connection) {

    if (connection == NULL) {
        perror("INSERT USER: CONNECTION NULL ERROR\n");
        return -1;
    }
    mysql_query(connection,"SET NAMES utf8");

    char insertSql[SQL_LENGTH_MAX];
    memset(insertSql, '\0', sizeof(insertSql));
    sprintf(insertSql, "INSERT INTO linpop.user(userNickName, userPassword, userSignature, userAvatar, userIp) \n"
                       "VALUES(\'%s\', \'%s\', \'%s\', \'%s\', %d);",
            user->userNickName, user->userPassword, user->userSignature, user->userAvatar, user->userIp);
    if (mysql_real_query(connection, insertSql, strlen(insertSql))) {
        perror("INSERT USER: QUERY ERROR\n");
        return -1;
    } else {
        MYSQL_RES* res;
        MYSQL_ROW row;
        int lastId = 0;

        if (mysql_real_query(connection, SQL_SELECT_LAST_ID, strlen(SQL_SELECT_LAST_ID))) {
            perror("SELECT LAST ID AFTER INSERT USER: QUERY ERROR\n");
            return -1;
        } else {
            res = mysql_store_result(connection);
            if (res) {
                row = mysql_fetch_row(res);
                lastId = atoi(row[0]);
                user->userId = lastId;
            } else {
                perror("SELECT LAST ID AFTER INSERT USER: RES NULL ERROR\n");
                return -1;
            }

        }

        return lastId;
    }
}

Status updateUserString(int userId, char *string, MYSQL* connection, int type) {
    if (connection == NULL) {
        perror("UPDATE USER STRING: CONNECTION NULL ERROR\n");
        return -1;
    }
    mysql_query(connection,"SET NAMES utf8");

    char querySql[SQL_LENGTH_MAX];
    memset(querySql, '\0', sizeof(querySql));
    switch (type) {
        case 1:
            sprintf(querySql, "UPDATE linpop.user\n"
                              "SET userAvatar= \'%s\'\n"
                              "WHERE userId=%d;", string, userId);
            break;
        case 2:
            sprintf(querySql, "UPDATE linpop.user\n"
                              "SET userNickname= \'%s\'\n"
                              "WHERE userId=%d;", string, userId);
            break;
        case 3:
            sprintf(querySql, "UPDATE linpop.user\n"
                              "SET userSignature= \'%s\'\n"
                              "WHERE userId=%d;", string, userId);
            break;
        default:
            break;
    }

    if (mysql_real_query(connection, querySql, strlen(querySql))) {
        perror("UPDATE USER STRING: QUERY ERROR\n");
        return -1;
    } else {
        return 1;
    }
}

Status updateUserStatus(int userId, int status, MYSQL* connection) {
    if (connection == NULL) {
        perror("UPDATE USER STATUS: CONNECTION NULL ERROR\n");
        return -1;
    }
    mysql_query(connection,"SET NAMES utf8");

    char querySql[SQL_LENGTH_MAX];
    memset(querySql, '\0', sizeof(querySql));
    sprintf(querySql, "UPDATE linpop.user\n"
                      "SET userStatus= \'%d\'\n"
                      "WHERE userId=%d;", status, userId);

    if (mysql_real_query(connection, querySql, strlen(querySql))) {
        perror("UPDATE USER STATUS: QUERY ERROR\n");
        return -1;
    } else {
        return 1;
    }
}

Status updateUserIp(int userId, int ip, MYSQL* connection) {
    if (connection == NULL) {
        perror("UPDATE USER IP: CONNECTION NULL ERROR\n");
        return -1;
    }
    mysql_query(connection,"SET NAMES utf8");

    char querySql[SQL_LENGTH_MAX];
    memset(querySql, '\0', sizeof(querySql));
    sprintf(querySql, "UPDATE linpop.user\n"
                      "SET userIp= \'%d\'\n"
                      "WHERE userId=%d;", ip, userId);

    if (mysql_real_query(connection, querySql, strlen(querySql))) {
        perror("UPDATE USER IP: QUERY ERROR\n");
        return -1;
    } else {
        return 1;
    }
}

Status checkUserPassword(int userId, char* password, MYSQL* connection) {
    if (connection == NULL) {
        perror("CHECK USER PASSWORD: CONNECTION NULL ERROR\n");
        return -1;
    }
    mysql_query(connection,"SET NAMES utf8");

    char querySql[SQL_LENGTH_MAX];
    memset(querySql, '\0', sizeof(querySql));
    sprintf(querySql, "SELECT COUNT(userId) AS amount\n"
                      "FROM linpop.user\n"
                      "WHERE userId=%d and userPassword=\'%s\';", userId, password);

    MYSQL_RES* res;
    MYSQL_ROW row;

    if (mysql_real_query(connection, querySql, strlen(querySql))) {
        perror("CHECK USER PASSWORD: QUERY ERROR\n");
    } else {
        res = mysql_store_result(connection);
        if (res) {
            row = mysql_fetch_row(res);
            if (row) {
                return row[0][0] - '0' == 0 ? 0 : 1;
            }
        } else {
            perror("CHECK USER PASSWORD: RES NULL ERROR\n");
        }
    }
    return -1;
}

Status updateUserFriendNum(int userId, MYSQL *connection, int type) {
    if (connection == NULL) {
        perror("UPDATE USER FRIEND NUM: CONNECTION NULL ERROR\n");
        return -1;
    }
    mysql_query(connection,"SET NAMES utf8");

    char querySql[SQL_LENGTH_MAX];
    memset(querySql, '\0', sizeof(querySql));
    switch (type) {
        case INCREASE:
            sprintf(querySql, "UPDATE linpop.user\n"
                              "SET userFriNum=userFriNum+1\n"
                              "WHERE userId=%d;", userId);
            break;
        case DECREASE:
            sprintf(querySql, "UPDATE linpop.user\n"
                              "SET userFriNum=userFriNum-1\n"
                              "WHERE userId=%d;", userId);
            break;
        default:
            break;
    }

    if (mysql_real_query(connection, querySql, strlen(querySql))) {
        perror("UPDATE USER FRIEND NUM: QUERY ERROR\n");
        return -1;
    } else {
        perror("query update userFriNum success\n");
        return 1;
    }
}