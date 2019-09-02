//
// Created by junqi on 2019/8/30.
//

#ifndef LINPOP_DATABASE_GROUPMESSAGE_H
#define LINPOP_DATABASE_GROUPMESSAGE_H

#include "DATABASE_mysql.h"

/**
 * 这里还要添加DATETIME类型
 */
typedef struct {
    int gmId;
    char *gmContent;
    int gmFromId;
    int gmGroupId;
    char* gmDateTime;
} GroupMessage;

typedef struct {
    GroupMessage* gmMsgs;   //group_message表中的 msgs
    int gmNum;
} GroupMessageList;

/**
 *
 * @param groupMessage
 */
void freeGroupMessage(GroupMessage *groupMessage);

/**
 *
 * @param groupMessageList
 */
void freeGroupMessageList(GroupMessageList groupMessageList);

/**
 * @param a GroupMessage pointer need to be allocated memory to
 */
void mallocGroupMessage(GroupMessage *groupMessage);

/**
 * 在发送数据时调用此函数. 向数据库中插入这条消息.
 *
 * must assign: msg->gmGroupId, msg->gmContent, msg->gmFromId
 *
 * gmId将会自动生成. gmFromId为发送者的ID, gmGroupId为接受者的ID
 * @param msg 要插入消息的指针
 * @param mysql 数据库链接句柄
 * @param connection　链接后的句柄
 * @return 成功插入返回gmId, 插入失败返回-1
 */
int insertGmMsg(GroupMessage *msg, MYSQL* connection);

/**
 * 需要获得群聊记录时调用
 * 数据库查询数据时要同时获得消息条数
 * @param groupId 群ID
 * @param connection　链接后的句柄
 * @return 有信息返回GroupMessageList, 包括gmMsgs数组和信息数目gmNum.
 * @return when there is no groupList or query fail, return GroupMessageList(gmMsgs = NULL, gmNum = 0)
 */
GroupMessageList getGmMsgList(int groupId, MYSQL* connection);

#endif //LINPOP_DATABASE_GROUPMESSAGE_H
