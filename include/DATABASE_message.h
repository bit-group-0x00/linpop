//
// Created by junqi on 2019/8/30.
//

#ifndef LINPOP_DATABASE_MESSAGE_H
#define LINPOP_DATABASE_MESSAGE_H

#include <stdbool.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include "DATABASE_mysql.h"

typedef struct {
    int msgId;
    char *msgContent;
    char *msgDateTime;
    Status msgStatus;
    int msgFromId;
    int msgToId;
} Message;

typedef struct {
    Message* msgs;
    int msgNum;
} MessageList;

/**
 * free a messageList which is no need to be used
 *
 * @param messageList the messageList needed to be free
 */
void freeMsgList(MessageList messageList);

/**
 * 在发送数据时调用此函数. 向数据库中插入这条消息.
 *
 * must assign:
 * msgContent, msgFromId, msgToId.
 *
 * msgStatus初始置为false, 表示还未被接收到.
 * msgFromId为发送者的ID, msgToId为接受者的ID
 * @param msg 要插入消息的指针
 * @param connection 连接数据库句柄
 * @return 成功插入返回msgId, 插入失败返回-1
 */
int insertMsg(Message *msg, MYSQL *connection);

/**
 * 将消息由未读状态更新成已读状态
 * @param msgId 消息的ID
 * @param connection 连接数据库句柄
 * @return 1: 更新成功, -1: 更新失败
 */
Status updateMsgStatus(int msgId, MYSQL *connection);

/**
 * 需要获得两个用户之间的聊天记录时调用
 * 数据库查询数据时要同时获得消息条数
 * @param userId1 第一个用户的ID
 * @param userId2 第二个用户的ID
 * @param connection 连接数据库句柄
 * @return 有信息返回MessageList, 包括messages数组和message数目.
 * @return when there is no message or ERROR occur, return empty MessageList(msgs = NULL, msgNum = 0)
 */
MessageList getMsgList(int userId1, int userId2, MYSQL *connection);

#endif //LINPOP_DATABASE_MESSAGE_H
