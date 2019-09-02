//
// Created by new on 9/1/19.
//
#include "../include/DATABASE_groupUser.h"
Status insertGroupUser(int groupId, int groupUserId, MYSQL* connection)
{
    char insertMessageSql[200];
    memset(insertMessageSql, '\0', sizeof(insertMessageSql));
    mysql_query(connection, "SET names utf8");
    sprintf(insertMessageSql, "INSERT INTO User.group_user(guGroupId, guUserId) VALUES(%d, %d);", groupId, groupUserId);
    if(mysql_real_query(connection, insertMessageSql, strlen(insertMessageSql)))
    {
        printf("Insert fail of groupUser\n");
        return 0;
    }
    else
    {
        printf("Insert success of groupUser\n");
        return 1;
    }
}

Status deleteGroupUser(int groupId, int groupUserId, MYSQL* connection)
{
    char deleteSql[200];
    memset(deleteSql, '\0', sizeof(deleteSql));
    mysql_query(connection, "SET names utf8");
    sprintf(deleteSql, "DELETE FROM User.group_user WHERE guGroupId=%d and guUserId=%d;",groupId, groupUserId);
    if(mysql_real_query(connection, deleteSql, strlen(deleteSql)))
    {
        printf("Delete fail of groupUser\n");
        return 0;
    }
    else
    {
        printf("Delete success of groupUser\n");
        return 1;
    }
}
