//
// Created by new on 9/1/19.
//
#include "../include/DATABASE_group.h"
int insertGroup(Group* group, MYSQL* connection, int userId)
{
    char insertGroupSql[200];
    MYSQL_RES *res;
    MYSQL_ROW row;
    int checkInsertGroupUser;
    memset(insertGroupSql, '\0', sizeof(insertGroupSql));
    mysql_query(connection, "SET NAMES utf8");
    sprintf(insertGroupSql, "INSERT INTO User._group(groupName, groupIntro, groupIcon) VALUES(\"%s\", \"%s\", \"%s\");", group->groupName, group->groupIntro, group->groupIcon);
    if(mysql_real_query(connection, insertGroupSql, strlen(insertGroupSql)))
    {
        printf("Insert failure of _group\n");
        return -1;
    }
    else
    {
        if(mysql_real_query(connection, SELECT_LAST_ID, strlen(SELECT_LAST_ID)))
        {
            printf("Failure of getting ID In groupMessage\n");
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
        insertGroupUser(group->groupId, userId, connection);
        printf("Insert success of _group\n");
        return group->groupId;
    }
}

Status updateGroupName(int groupId, char *groupName, MYSQL* connection)
{
    char updateNameSql[200];
    memset(updateNameSql, '\0', sizeof(updateNameSql));
    mysql_query(connection, "SET names utf8");
    sprintf(updateNameSql, "UPDATE User._group SET groupName =\"%s\" WHERE groupId=%d;", groupName, groupId);
    if(mysql_real_query(connection, updateNameSql, strlen(updateNameSql)))
    {
        printf("Update fail of group\n");
        return false;
    }
    else
    {
        printf("Update success of group\n");
        return true;
    }
}

Status updateGroupIntro(int groupId, char *groupIntro, MYSQL* connection)
{
    char updateGroupIntroSql[200];
    memset(updateGroupIntroSql, '\0', sizeof(updateGroupIntroSql));
    mysql_query(connection, "SET names utf8");
    sprintf(updateGroupIntroSql, "UPDATE User._group SET groupIntro =\"%s\" WHERE groupId=%d;", groupIntro, groupId);
    if(mysql_real_query(connection, updateGroupIntroSql, strlen(updateGroupIntroSql)))
    {
        printf("Update fail of group\n");
        return false;
    }
    else
    {
        printf("Update success of group\n");
        return true;
    }
}

Status updateGroupIcon(int groupId, char *groupIcon, MYSQL* connection)
{
    char updateGroupIconSql[200];
    memset(updateGroupIconSql, '\0', sizeof(updateGroupIconSql));
    mysql_query(connection, "SET names utf8");
    sprintf(updateGroupIconSql, "UPDATE User._group SET groupIcon =\"%s\" WHERE groupId=%d;", groupIcon, groupId);
    if(mysql_real_query(connection, updateGroupIconSql, strlen(updateGroupIconSql)))
    {
        printf("Update fail of group\n");
        return false;
    }
    else
    {
        printf("Update success of group\n");
        return true;
    }
}

Status deleteGroup(int groupId, MYSQL* connection)
{
    char deleteSql[200];
    memset(deleteSql, '\0', sizeof(deleteSql));
    mysql_query(connection, "SET names utf8");
    sprintf(deleteSql, "DELETE FROM User._group WHERE groupId=%d;", groupId);
    if(mysql_real_query(connection, deleteSql, strlen(deleteSql)))
    {
        printf("Delete fail of group\n");
        return false;
    }
    else
    {
        printf("Delete success of group\n");
        return true;
    }
}