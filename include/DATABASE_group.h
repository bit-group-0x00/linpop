//
// Created by junqi on 2019/8/30.
//

#ifndef LINPOP_DATABASE_GROUP_H
#define LINPOP_DATABASE_GROUP_H

#include <mysql/mysql.h>
#include "DATABASE_mysql.h"

typedef struct {
    int groupId;
    char *groupName;
    char *groupIntro;
    char *groupIcon;
} Group;

/**
 * 新建一个聊天群组的时候需要调用此函数,
 * 同时需要调用DATABASE_groupUser中的insertGroupUser()
 *
 * must assign: groupName, groupIntro, groupIcon.
 *
 * groupId会自动生成. groupIntro为群聊简介
 * groupIcon为群聊头像
 * @param group 将要添加进数据库的Group pointer
 * @param connection　链接后的句柄
 * @param userId 用户的Id
 * @return 成功创建: 返回groupId, 创建失败: 返回-1
 */
int insertGroup(Group* group, MYSQL* connection);

/**
 * 更新群聊名
 * @param groupId
 * @param groupName
 * @param connection　链接后的句柄
 * @return 1: success -1: error occur
 */
Status updateGroupName(int groupId, char *groupName, MYSQL* connection);

/**
 * 更新群简介
 * @param groupId
 * @param groupIntro
 * @param connection　链接后的句柄
 * @return 1: success -1: error occur
 */
Status updateGroupIntro(int groupId, char *groupIntro, MYSQL* connection);

/**
 * 更新群聊头像(先放着, 用到了再写)
 * @param groupId
 * @param groupIcon 新的群聊头像在服务器中的路径
 * @param connection　链接后的句柄
 * @return 1: success -1 error occur
 */
Status updateGroupIcon(int groupId, char *groupIcon, MYSQL* connection);

/**
 * 删除聊天群组时调用
 * @param groupId 将要删除群组的ID
 * @param connection　链接后的句柄
 * @return 1: 成功, -1: 失败
 */
Status deleteGroup(int groupId, MYSQL* connection);
#endif //LINPOP_DATABASE_GROUP_H
