//
// Created by junqi on 2019/8/30.
//

#ifndef LINPOP_DATABASE_MESSAGE_H
#define LINPOP_DATABASE_MESSAGE_H
#define SELECT_LAST_ID "SELECT LAST_INSERT_ID();"

#include <stdbool.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
typedef int Status;

/**
 * 这里还要添加DATETIME类型
 */
typedef struct {
    int msgId;
    char *msgContent;
    Status msgStatus;
    int msgFromId;
    int msgToId;
} Message;

typedef struct {
    Message* messages;
    int msgNum;
} MessageList;

/**
 * 在发送数据时调用此函数. 向数据库中插入这条消息.
 * msgId将会自动生成. msgStatus初始置为false, 表示还未被接收到.
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
 * @return true: 更新成功, false: 更新失败
 */
Status updateMsgStatus(int msgId, MYSQL *connection);

/**
 * 需要获得两个用户之间的聊天记录时调用
 * 数据库查询数据时要同时获得消息条数
 * @param userId1 第一个用户的ID
 * @param userId2 第二个用户的ID
 * @param connection 连接数据库句柄
 * @return 有信息返回MessageList, 包括messages数组和message数目. 无信息时返回NULL.
 */
MessageList getMsgList(int userId1, int userId2, MYSQL *connection);

#endif //LINPOP_DATABASE_MESSAGE_H
