//
// Created by junqi on 2019/8/30.
//

#ifndef LINPOP_DATABASE_GROUP_H
#define LINPOP_DATABASE_GROUP_H

#include <stdbool.h>
typedef int Status;

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
 * groupId会自动生成. groupIntro为群聊简介
 * groupIcon为群聊头像
 * @param group 将要添加进数据库的group元组
 * @return 成功创建: 返回groupId, 创建失败: 返回-1
 */
int insertGroup(Group* group);

/**
 * 更新群聊名(先放着, 用到了再写)
 * @param groupId
 * @param groupName
 * @return
 */
Status updateGroupName(int groupId, char *groupName);

/**
 * 更新群简介(先放着, 用到了再写)
 * @param groupId
 * @param groupIntro
 * @return
 */
Status updateGroupIntro(int groupId, char *groupIntro);

/**
 * 更新群聊头像(先放着, 用到了再写)
 * @param groupId
 * @param groupIcon 新的群聊头像在服务器中的路径
 * @return
 */
Status updateGroupIcon(int groupId, char *groupIcon);

/**
 * 删除聊天群组(如果能做到这一步)时调用
 * @param groupId 将要删除群组的ID
 * @return true: 成功, false: 失败
 */
Status deleteGroup(int groupId);
#endif //LINPOP_DATABASE_GROUP_H
