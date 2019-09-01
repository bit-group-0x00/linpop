//
// Created by junqi on 2019/8/30.
//

#ifndef LINPOP_DATABASE_GROUPUSER_H
#define LINPOP_DATABASE_GROUPUSER_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <mysql/mysql.h>
typedef int Status;

typedef struct {
    int groupId;
    int groupUserId;
} GroupUser;

/**
 * 在创建群聊或添加新成员进入群聊时调用此函数
 * @param groupId 群ID
 * @param groupUserId 新成员ID
 * @param connection　链接后的句柄
 * @return true: 加入成功. false: 加入失败
 */
Status insertGroupUser(int groupId, int groupUserId, MYSQL* connection);

/**
 * 在退出群聊时调用此函数
 * @param groupId 群ID
 * @param groupUserId 将要退出的成员ID
 * @param connection　链接后的句柄
 * @return true: 删除成功 false: 删除失败
 */
Status deleteGroupUser(int groupId, int groupUserId, MYSQL* connection);

#endif //LINPOP_DATABASE_GROUPUSER_H
