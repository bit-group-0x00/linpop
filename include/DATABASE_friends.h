//
// Created by junqi on 2019/8/30.
//

#ifndef LINPOP_DATABASE_FRIENDS_H
#define LINPOP_DATABASE_FRIENDS_H

#include "DATABASE_user.h"
#include "DATABASE_mysql.h"

typedef struct {
    int friId;
    int friUserId1;
    int friUserId2;
} Friends;

typedef struct {
    int userId; //用户ID
    int *friId; //该用户的好友ID数组的头指针
    int friendsNum;
} FriendList;

/**
 * 判断两个人是否是好友关系.
 * 使用者此函数时, 可将userId1和userId2看成等价的, 无需考虑顺序问题.
 * @param userId1 第一个人的id
 * @param userId2 第二个人的id
 * @return 1: 是好友; 0: 不是好友; -1: query fail
 */
Status isFriends(int userId1, int userId2, MYSQL* connection);

/**
 * 当添加好友成功之后调用此函数向数据库中插入好友对.
 * 对调用此借口的人来说, userId1与userId2等价且无需考虑friId.
 *
 * 对编写此函数的人来说, userId1必须保证小于userId2. friId会自动生成.
 * 同时还需要对user表中的userFriNum进行更新.
 * @param userId1 第一个人的ID
 * @param userId2 第二个人的ID
 * @return 1: 成功插入数据库, -1: 失败
 */
Status insertFriends(int userId1, int userId2, MYSQL* connection);

/**
 * 当需要获得用户的好友列表的时候调用此函数.
 *
 * 用SUM计算好友总数
 * @param userId 用户的ID
 * @return FriendList 用户的好友列表, 包括用户ID, 好友ID数组头指针以及好友数
 * @return if user has no friend or query error occur, return friendList(friId == NULL, friendsNum == 0)
 */
FriendList getFriList(int userId, MYSQL* connection);

/**
 * 当删除好友(如果能做到这里的话)时, 执行此操作.
 * 两个传入参数ID的注意事项和insertFriends()一样
 * 同样也需要对user表中的userFriNum进行更新
 * @param userId1 第一个人的ID
 * @param userId2 第二个人的ID
 * @return 1: 删除成功, -1: 删除失败
 */
Status deleteFriends(int userId1, int userId2, MYSQL *connection);

/**
 * release the memory occupied by friendList.friId
 * @param friendList
 */
void freeFriList(FriendList friendList);
#endif //LINPOP_DATABASE_FRIENDS_H
