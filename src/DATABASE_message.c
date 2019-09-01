//
// Created by new on 9/1/19.
//
#include "../include/DATABASE_message.h"
int insertMsg(Message *msg, MYSQL *connection)
{
    char insertMessageSql[200];
    MYSQL_RES *res;
    MYSQL_ROW row;
    memset(insertMessageSql, '\0', sizeof(insertMessageSql));
    mysql_query(connection, "SET names utf8");
    sprintf(insertMessageSql, "insert into User.message(msgContent, msgFromId, msgToId, msgDateTime) values(\"%s\", %d, %d, now());", msg->msgContent, msg->msgFromId, msg->msgToId);
    if(mysql_real_query(connection, insertMessageSql, strlen(insertMessageSql)))
    {
        printf("Insert failure of message\n");
        return -1;
    }
    else
    {
        if(mysql_real_query(connection, SELECT_LAST_ID, strlen(SELECT_LAST_ID)))
        {
            printf("Failure of getting ID In message\n");
            return -1;
        }
        else
        {
            res = mysql_store_result(connection);
            if(res)
            {
                row = mysql_fetch_row(res);
                msg->msgId = atoi(row[0]);
            }
        }
        printf("Insert success of message\n");
        return msg->msgId;
    }
}

Status updateMsgStatus(int msgId, MYSQL *connection)
{
    char updateMsgStatusSql[200];
    memset(updateMsgStatusSql, '\0', sizeof(updateMsgStatusSql));
    mysql_query(connection, "SET names utf8");
    sprintf(updateMsgStatusSql, "UPDATE User.message SET msgStatus = 1 WHERE msgId=%d;", msgId);
    if(mysql_real_query(connection, updateMsgStatusSql, strlen(updateMsgStatusSql)))
    {
        printf("Update fail of message\n");
        return false;
    }
    else
    {
        printf("Update success of message\n");
        return true;
    }
}

MessageList getMsgList(int userId1, int userId2, MYSQL *connection)
{
    int countMessage;
    MessageList result;
    Message *go;
    MYSQL_RES* res;
    MYSQL_ROW row;
    result.msgNum = 0;
    char strSelectSql[200];
    mysql_query(connection, "SET names utf8");
    sprintf(strSelectSql, "SELECT * FROM User.message WHERE msgFromId=%d and msgToId=%d;", userId1, userId2);
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
                result.messages = NULL;
                return result;
            }
            result.messages = (Message *)malloc(sizeof(result.messages) * countMessage);
            go = result.messages;
            row = mysql_fetch_row(res);
            while(row)
            {
                go->msgContent = (char *)malloc(sizeof(char) * 1000);
                go->msgId = atoi(row[0]);
                strcpy(go->msgContent, row[1]);
                go->msgFromId = atoi(row[4]);
                go->msgToId = atoi(row[5]);
                go++;
                row = mysql_fetch_row(res);
                result.msgNum++;
            }
        }
        return result;
    }
}

