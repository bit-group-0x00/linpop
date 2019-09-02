//
// Created by new on 9/1/19.
//
#include "../include/DATABASE_group.h"

Group* mallocGroup() {
    Group *group;

    group = (Group *) malloc(sizeof(Group));
    group->groupName = (char *) malloc(sizeof(char) * 30);
    memset(group->groupName, 0, 30);
    group->groupIntro = (char *) malloc(sizeof(char) * 300);
    memset(group->groupIntro, 0, 300);
    group->groupIcon = (char *) malloc(sizeof(char) * 300);
    memset(group->groupIcon, 0, 300);
    return group;
}

void freeGroup(Group *group) {
    if (group == NULL) {
        return;
    }
    if (group->groupIcon != NULL) {
        free(group->groupIcon);
        group->groupIcon = NULL;
    }
    if (group->groupIntro != NULL) {
        free(group->groupIntro);
        group->groupIntro = NULL;
    }
    if (group->groupName != NULL) {
        free(group->groupName);
        group->groupName = NULL;
    }
    free(group);
}

int insertGroup(Group* group, MYSQL* connection)
{
    if (connection == NULL) {
        perror("INSERT GROUP: CONNECTION NULL ERROR\n");
        return -1;
    }
    char insertGroupSql[200];
    MYSQL_RES *res;
    MYSQL_ROW row;
    memset(insertGroupSql, '\0', sizeof(insertGroupSql));
    mysql_query(connection, "SET NAMES utf8");
    sprintf(insertGroupSql, "INSERT INTO linpop._group(groupName, groupIntro, groupIcon)\n"
                            "VALUES(\'%s\', \'%s\', \'%s\');",
            group->groupName, group->groupIntro, group->groupIcon);

    if(mysql_real_query(connection, insertGroupSql, strlen(insertGroupSql)))
    {
        perror("INSERT GROUP: QUERY ERROR\n");
        return -1;
    }
    else
    {
        if(mysql_real_query(connection, SELECT_LAST_ID, strlen(SELECT_LAST_ID)))
        {
            perror("SELECT LAST ID AFTER INSERT GROUP: QUERY ERROR\n");
            return -1;
        }
        else
        {
            res = mysql_store_result(connection);
            if(res)
            {
                row = mysql_fetch_row(res);
                group->groupId = atoi(row[0]);
            }
        }
        return group->groupId;
    }
}

Status updateGroupName(int groupId, char *groupName, MYSQL* connection)
{
    if (connection == NULL) {
        perror("UPDATE GROUP NAME: CONNECTION NULL ERROR\n");
        return -1;
    }
    char updateNameSql[200];
    memset(updateNameSql, '\0', sizeof(updateNameSql));
    mysql_query(connection, "SET NAMES utf8");
    sprintf(updateNameSql, "UPDATE linpop._group\n"
                           "SET groupName =\"%s\"\n"
                           "WHERE groupId=%d;", groupName, groupId);
    if(mysql_real_query(connection, updateNameSql, strlen(updateNameSql)))
    {
        perror("UPDATE GROUP NAME: QUERY ERROR");
        return -1;
    }
    else
    {
        return 1;
    }
}

Status updateGroupIntro(int groupId, char *groupIntro, MYSQL* connection)
{
    if (connection == NULL) {
        perror("UPDATE GROUP INTRODUCTION: CONNECTION NULL ERROR\n");
        return -1;
    }
    char updateGroupIntroSql[200];
    memset(updateGroupIntroSql, '\0', sizeof(updateGroupIntroSql));
    mysql_query(connection, "SET NAMES utf8");
    sprintf(updateGroupIntroSql, "UPDATE linpop._group\n"
                                 "SET groupIntro =\"%s\"\n"
                                 "WHERE groupId=%d;", groupIntro, groupId);
    if(mysql_real_query(connection, updateGroupIntroSql, strlen(updateGroupIntroSql)))
    {
        perror("UPDATE GROUP INTRODUCTION: QUERY ERROR");
        return -1;
    }
    return 1;
}

Status updateGroupIcon(int groupId, char *groupIcon, MYSQL* connection)
{
    if (connection == NULL) {
        perror("UPDATE GROUP ICON: CONNECTION NULL ERROR\n");
        return -1;
    }
    char updateGroupIconSql[200];
    memset(updateGroupIconSql, '\0', sizeof(updateGroupIconSql));
    mysql_query(connection, "SET NAMES utf8");
    sprintf(updateGroupIconSql, "UPDATE linpop._group\n"
                                "SET groupIcon =\"%s\"\n"
                                "WHERE groupId=%d;", groupIcon, groupId);
    if(mysql_real_query(connection, updateGroupIconSql, strlen(updateGroupIconSql)))
    {
        perror("UPDATE GROUP ICON: QUERY ERROR\n");
        return -1;
    }
    return 1;
}

Status deleteGroup(int groupId, MYSQL* connection)
{
    if (connection == NULL) {
        perror("DELETE GROUP ERROR: CONNECTION NULL ERROR\n");
        return -1;
    }
    char deleteSql[200];
    memset(deleteSql, '\0', sizeof(deleteSql));
    mysql_query(connection, "SET NAMES utf8");
    sprintf(deleteSql, "DELETE FROM linpop._group\n"
                       "WHERE groupId=%d;", groupId);
    if(mysql_real_query(connection, deleteSql, strlen(deleteSql)))
    {
        perror("DELETE GROUP ERROR: QUERY ERROR\n");
        return -1;
    }
    return 1;
}

Group* getGroupInfoByGroupId(int groupId, MYSQL *connection) {
    if (connection == NULL) {
        perror("GET GROUP BY ID: CONNECTION NULL ERROR\n");
        return NULL;
    }
    mysql_query(connection,"SET NAMES utf8");
    char querySql[SQL_LENGTH_MAX];
    memset(querySql, 0, sizeof(querySql));
    sprintf(querySql, "SELECT *\n"
                      "FROM linpop._group\n"
                      "WHERE groupId=%d;", groupId);
    MYSQL_RES* res;
    MYSQL_ROW row;

    if (mysql_real_query(connection, querySql, strlen(querySql))) {
        perror("GET GROUP BY ID: QUERY ERROR\n");
    } else {
        res = mysql_store_result(connection);
        if (res) {
            row = mysql_fetch_row(res);
            if (row) {
                Group *group = mallocGroup();
                group->groupId = groupId;
                strcpy(group->groupName, row[1]);
                strcpy(group->groupIntro, row[2]);
                strcpy(group->groupIcon, row[3]);
                return group;
            }
        } else {
            perror("GET GROUP BY ID: RES NULL ERROR\n");
            return NULL;
        }
    }
    return NULL;
}