//
// Created by new on 8/31/19.
//
#include "../include/DATABASE_groupMessage.h"
#include "../include/DATABASE_mysql.h"

void freeGroupMessage(GroupMessage *groupMessage) {
    if (groupMessage == NULL) {
        return;
    }
    if (groupMessage->gmDateTime != NULL) {
        free(groupMessage->gmDateTime);
        groupMessage->gmDateTime = NULL;
    }
    if (groupMessage->gmContent != NULL) {
        free(groupMessage->gmContent);
        groupMessage->gmContent = NULL;
    }
    groupMessage = NULL;
}

void freeGroupMessageList(GroupMessageList groupMessageList) {
    for (int i = 0; i < groupMessageList.gmNum; i++) {
        freeGroupMessage(groupMessageList.gmMsgs + i);
    }
}

//void mallocGroupMessage(GroupMessage *groupMessage) {
//
//    groupMessage = (GroupMessage *) malloc(sizeof(GroupMessage));
//    groupMessage->gmContent = (char *) malloc(sizeof(char) * MESSAGE_MAX_LENGTH);
//    groupMessage->gmDateTime = (char *) malloc(sizeof(char) * DATETIME_LENGTH);
//}

int insertGmMsg(GroupMessage *msg, MYSQL* connection)
{
    char insertMessageSql[200];
    MYSQL_RES *res;
    MYSQL_ROW row;
    memset(insertMessageSql, '\0', sizeof(insertMessageSql));
    mysql_query(connection, "SET NAMES utf8");
    sprintf(insertMessageSql,
            "INSERT INTO linpop.group_message(gmGroupID, gmContent, gmFromId, gmDateTime)\n"
            "VALUES(%d, \"%s\", %d, NOW());",
            msg->gmGroupId, msg->gmContent, msg->gmFromId);

    if(mysql_real_query(connection, insertMessageSql, strlen(insertMessageSql)))
    {
        perror("INSERT GROUP MESSAGE: QUERY ERROR\n");
        return -1;
    }
    else
    {
        if(mysql_real_query(connection, SQL_SELECT_LAST_ID, strlen(SQL_SELECT_LAST_ID)))
        {
            perror("SELECT LAST ID AFTER INSERT GROUP MESSAGE: QUERY ERROR\n");
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
    }
    return  msg->gmId;
}

GroupMessageList getGmMsgList(int groupId, MYSQL* connection)
{
    GroupMessageList groupMessageList;
    groupMessageList.gmNum = 0;
    groupMessageList.gmMsgs = NULL;

    if (connection == NULL) {
        perror("GET GROUP MESSAGE LIST ERROR");
        return groupMessageList;
    }

    MYSQL_RES* res;
    MYSQL_ROW row;
    char strSelectSql[200];
    sprintf(strSelectSql, "SELECT * FROM linpop.group_message\n"
                          "WHERE gmGroupId=%d;", groupId);

    if(mysql_real_query(connection, strSelectSql, strlen(strSelectSql)))
    {
        perror("GET GROUP MESSAGE LIST: QUERY ERROR\n");
        return groupMessageList;
    }
    else
    {
        mysql_query(connection, "SET NAMES utf8");
        res = mysql_store_result(connection);
        if(res)
        {
            groupMessageList.gmNum = mysql_num_rows(res);
            groupMessageList.gmMsgs = (GroupMessage *) malloc(sizeof(groupMessageList.gmMsgs) * groupMessageList.gmNum);

            row = mysql_fetch_row(res);
            int index = 0;
            while(row)
            {
                (groupMessageList.gmMsgs + index)->gmContent = (char *) malloc(sizeof(char) * 300);
                memset((groupMessageList.gmMsgs + index)->gmContent, 0, 300);
                (groupMessageList.gmMsgs + index)->gmDateTime = (char *) malloc(sizeof(char) * 50);
                memset((groupMessageList.gmMsgs + index)->gmDateTime, 0, 50);

                (groupMessageList.gmMsgs + index)->gmId = atoi(row[0]);
                (groupMessageList.gmMsgs + index)->gmGroupId = atoi(row[1]);
                strcpy((groupMessageList.gmMsgs + index)->gmContent, row[2]);
                (groupMessageList.gmMsgs + index)->gmFromId = atoi(row[3]);
                strcpy((groupMessageList.gmMsgs + index)->gmDateTime, row[4]);
                index++;
                row = mysql_fetch_row(res);
            }
        }
        return groupMessageList;
    }
}
