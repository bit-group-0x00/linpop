//
// Created by junqi on 2019/8/30.
//

#ifndef LINPOP_DATABASE_GROUPUSER_H
#define LINPOP_DATABASE_GROUPUSER_H

#include "DATABASE_mysql.h"

typedef struct {
    int groupId;
    int groupUserId;
} GroupUser;

typedef struct {
    int userId;
    int* groupIds;
    int groupNum;
} GroupList;

typedef struct {
    int groupId;
    int *userIds;
    int userNum;
}GroupUserList;

/**
 * free groupList.groupIds' memory
 * @param groupList
 */
void freeGroupList(GroupList groupList);

/**
 * free groupUserList.userIds' memory
 * @param groupUserList
 */
void freeGroupUserList(GroupUserList groupUserList);

/**
 * 在创建群聊或添加新成员进入群聊时调用此函数
 * @param groupId 群ID
 * @param groupUserId 新成员ID
 * @param connection　链接后的句柄
 * @return 1: 加入成功. -1: 加入失败
 */
Status insertGroupUser(int groupId, int groupUserId, MYSQL* connection);

/**
 * 在退出群聊时调用此函数
 * @param groupId 群ID
 * @param groupUserId 将要退出的成员ID
 * @param connection　链接后的句柄
 * @return 1: 删除成功 -1: 删除失败
 */
Status deleteGroupUser(int groupId, int groupUserId, MYSQL* connection);

/**
 *
 * @param userId
 * @param connection
 * @return GroupList include the num of user's groups and their groupIds.
 * @return when query error occur or user has no group: groupList.groupNum = 0, groupList.groupIds = NULL;
 */
GroupList getGroupListByUserId(int userId, MYSQL* connection);

/**
 *
 * @param groupId
 * @param connection
 * @return GroupUserList include the num of users int the specified group, and all user IDs.
 * @return when query error occur or group has no users: groupUserList.UserNum = 0, groupUserList.userIds = NULL;
 */
GroupUserList getUserListByGroupId(int groupId, MYSQL* connection);

#endif //LINPOP_DATABASE_GROUPUSER_H
