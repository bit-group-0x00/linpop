//
// Created by new on 8/31/19.
//
#include"../include/DATABASE_groupMessage.h"
#include "../include/DATABASE_mysql.h"
int insertGmMsg(GroupMessage *msg, MYSQL* connection)
{
    char insertMessageSql[200];
    MYSQL_RES *res;
    MYSQL_ROW row;
    memset(insertMessageSql, '\0', sizeof(insertMessageSql));
    mysql_query(connection, "SET names utf8");
    sprintf(insertMessageSql, "insert into User.group_message(gmGroupID, gmContent, gmFromId, gmDateTime) values(%d, \"%s\", %d, now());", msg->gmGroupId, msg->gmContent, msg->gmFromId);
    if(mysql_real_query(connection, insertMessageSql, strlen(insertMessageSql)))
    {
        printf("Insert failure of groupMessage\n");
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
                msg->gmId = atoi(row[0]);
            }
        }
        printf("Insert success of groupMessage\n");
    }
    return  msg->gmId;
}

GroupMessageList getGmMsgList(int groupId, MYSQL* connection)
{
    int countMessage;
    GroupMessageList result;
    GroupMessage *go;
    MYSQL_RES* res;
    MYSQL_ROW row;
    result.gmNum = 0;
    char strSelectSql[200];
    sprintf(strSelectSql, "SELECT * FROM User.group_message WHERE gmGroupId=%d;", groupId);
    if(mysql_real_query(connection, strSelectSql, strlen(strSelectSql)))
    {
        printf("error of exec of groupMessage\n");
        exit(1);
    }
    else
    {
        mysql_query(connection, "SET names utf8");
        res = mysql_store_result(connection);
        if(res)
        {
            countMessage = mysql_num_rows(res);
            if(countMessage == 0)
            {
                result.gmMsgs = NULL;
                return result;
            }
            result.gmMsgs = (GroupMessage *)malloc(sizeof(result.gmMsgs) * countMessage);
            go = result.gmMsgs;
            row = mysql_fetch_row(res);
            while(row)
            {
                go->gmContent = (char *)malloc(sizeof(char) * messageMaxSize);
                go->dataTime = (char *)malloc(sizeof(char) *dateTimeMaxSize);
                go->gmId = atoi(row[0]);
                go->gmGroupId = atoi(row[1]);
                strcpy(go->gmContent, row[2]);
                go->gmFromId = atoi(row[3]);
                strcpy(go->dataTime, row[4]);
                go++;
                row = mysql_fetch_row(res);
                result.gmNum++;
            }
        }
        return result;
    }
}
