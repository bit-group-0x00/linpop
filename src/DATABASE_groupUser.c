//
// Created by new on 9/1/19.
//
#include "../include/DATABASE_groupUser.h"

void freeGroupList(GroupList groupList) {
    if (groupList.groupIds != NULL) {
        free(groupList.groupIds);
        groupList.groupIds = NULL;
    }
}

Status insertGroupUser(int groupId, int groupUserId, MYSQL* connection)
{
    if (connection == NULL) {
        perror("INSERT GROUP USER: CONNECTION NULL ERROR");
        return -1;
    }

    char insertMessageSql[200];
    memset(insertMessageSql, '\0', sizeof(insertMessageSql));
    mysql_query(connection, "SET NAMES utf8");
    sprintf(insertMessageSql, "INSERT INTO User.group_user(guGroupId, guUserId)\n"
                              "VALUES(%d, %d);", groupId, groupUserId);

    if(mysql_real_query(connection, insertMessageSql, strlen(insertMessageSql)))
    {
        perror("INSERT GROUP USER: QUERY ERROR\n");
        return -1;
    }
    return 1;
}

Status deleteGroupUser(int groupId, int groupUserId, MYSQL* connection)
{
    if (connection == NULL) {
        perror("DELETE GROUP USER: CONNECTION NULL ERROR");
        return -1;
    }
    char deleteSql[200];
    memset(deleteSql, '\0', sizeof(deleteSql));
    mysql_query(connection, "SET NAMES utf8");
    sprintf(deleteSql, "DELETE FROM linpop.group_user\n"
                       "WHERE guGroupId=%d and guUserId=%d;",groupId, groupUserId);
    if(mysql_real_query(connection, deleteSql, strlen(deleteSql)))
    {
        printf("DELETE GROUP USER: QUERY ERROR\n");
        return -1;
    }
    return 1;
}

GroupList getGroupListByUserId(int userId, MYSQL *connection) {
    GroupList groupList;
    groupList.userId = userId;
    groupList.groupIds = NULL;
    groupList.groupNum = 0;

    if (connection == NULL) {
        perror("GET GROUP LIST: CONNECTION NULL ERROR");
        return groupList;
    }

    mysql_query(connection,"SET NAMES utf8");
    char querySql[SQL_LENGTH_MAX];
    memset(querySql, 0, sizeof(querySql));
    sprintf(querySql, "SELECT *\n"
                      "FROM linpop.group_user\n"
                      "WHERE guUserId=%d;", userId);

    MYSQL_RES* res;
    MYSQL_ROW row;
    if (mysql_real_query(connection, querySql, strlen(querySql))) {
        printf("GET GROUP LIST: QUERY ERROR\n");
    } else {
        res = mysql_store_result(connection);
        if (res) {
            groupList.groupNum = mysql_num_rows(res);
            row = mysql_fetch_row(res);
            int index = 0;
            while (row) {
                *(groupList.groupIds + index) = atoi(row[0]);
                index++;
                row = mysql_fetch_row(res);
            }
        } else {
            perror("GET GROUP LIST: RES NULL ERROR\n");
        }
    }
    return groupList;
}