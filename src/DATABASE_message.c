//
// Created by new on 9/1/19.
//
#include "../include/DATABASE_message.h"

void freeMsgList(MessageList messageList) {
    for (int i = 0; i < messageList.msgNum; i++) {
        free((messageList.msgs + i)->msgContent);
        (messageList.msgs + i)->msgContent = NULL;
        free((messageList.msgs + i)->msgDateTime);
        (messageList.msgs + i)->msgDateTime = NULL;
    }
    messageList.msgs = NULL;
}

char* insertMsg(Message *msg, MYSQL *connection)
{
    char *time = NULL;
    if (connection == NULL) {
        perror("INSERT MESSAGE: CONNECTION NULL ERROR");
        return NULL;
    }
    char insertMessageSql[200];
    MYSQL_RES *res;
    MYSQL_ROW row;
    memset(insertMessageSql, '\0', sizeof(insertMessageSql));
    mysql_query(connection, "SET names utf8");
    sprintf(insertMessageSql, "INSERT INTO linpop.message(msgContent, msgFromId, msgToId, msgDateTime)\n"
                              " VALUES(\'%s\', %d, %d, NOW());", msg->msgContent, msg->msgFromId, msg->msgToId);
    if(mysql_real_query(connection, insertMessageSql, strlen(insertMessageSql)))
    {
        perror("INSERT MESSAGE: QUERY ERROR\n");
        return NULL;
    }
    else
    {
        if (mysql_real_query(connection, "SELECT msgDateTime\n"
                                         "FROM linpop.message\n"
                                         "WHERE msgId=LAST_INSERT_ID();\n",
                             strlen("SELECT msgDateTime\n"
                                    "FROM linpop.message\n"
                                    "WHERE msgId=LAST_INSERT_ID();\n"))) {

            perror("QUERY LAST TIME ID AFTER INSERT MESSAGE: QUERY ERROR\n");
            return NULL;
        } else {
            res = mysql_store_result(connection);
            if (res) {
                row = mysql_fetch_row(res);
                time = (char *) malloc(sizeof(char) * 40);
                strcpy(time, row[0]);
            }
            return time;
        }
    }
}

Status updateMsgStatus(int msgId, MYSQL *connection)
{
    if (connection == NULL) {
        perror("UPDATE MESSAGE STATUS: CONNECTION NULL ERROR");
        return -1;
    }
    char updateMsgStatusSql[200];
    memset(updateMsgStatusSql, '\0', sizeof(updateMsgStatusSql));
    mysql_query(connection, "SET NAMES utf8");
    sprintf(updateMsgStatusSql, "UPDATE linpop.message\n"
                                "SET msgStatus = 1\n"
                                "WHERE msgId=%d;", msgId);
    if(mysql_real_query(connection, updateMsgStatusSql, strlen(updateMsgStatusSql)))
    {
        perror("UPDATE MESSAGE STATUS: QUERY ERROR");
        return -1;
    }

    return 1;
}

MessageList getMsgList(int userId1, int userId2, MYSQL *connection)
{
    MessageList messageList;
    messageList.msgs = NULL;
    messageList.msgNum = 0;
    if (connection == NULL) {
        perror("QUERY MESSAGE LIST: CONNECTION NULL ERROR");
        return messageList;
    }

    MYSQL_RES* res;
    MYSQL_ROW row;
    char querySql[200];
    mysql_query(connection, "SET NAMES utf8");
    sprintf(querySql, "SELECT * FROM linpop.message\n"
                      "WHERE (msgFromId=%d and msgToId=%d) OR (msgFromId=%d and msgToId=%d);",
            userId1, userId2, userId2, userId1);

    if(mysql_real_query(connection, querySql, strlen(querySql)))
    {
        perror("QUERY MESSAGE LIST: QUERY ERROR\n");
        return messageList;
    }
    else
    {
        mysql_query(connection, "SET NAMES utf8");
        res = mysql_store_result(connection);
        if(res)
        {
            messageList.msgNum = mysql_num_rows(res);
            messageList.msgs = (Message *) malloc(sizeof(Message) * messageList.msgNum);

            Message *go = messageList.msgs;
            row = mysql_fetch_row(res);
            while(row)
            {
                // TODO: reference for make message table
                go->msgContent = (char *) malloc(sizeof(char) * 1000);
                go->msgDateTime = (char *) malloc(sizeof(char) * 100);
                go->msgId = atoi(row[0]);
                strcpy(go->msgContent, row[1]);
                strcpy(go->msgDateTime, row[2]);
                go->msgStatus = row[3][0] == '0' ? 0 : 1;
                go->msgFromId = atoi(row[4]);
                go->msgToId = atoi(row[5]);
                go++;
                row = mysql_fetch_row(res);
            }
        }
        return messageList;
    }
}