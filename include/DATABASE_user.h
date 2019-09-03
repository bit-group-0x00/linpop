//
// Created by junqi on 2019/8/30.
//

#ifndef LINPOP_DATABASE_USER_H
#define LINPOP_DATABASE_USER_H

#include "DATABASE_mysql.h"

typedef struct
{
    int userId;
    int userFriNum;
    char* userNickName;
    char* userPassword;
    char* userSignature;
    Status userStatus;
    char* userAvatar;
    int userIp;
}User;

/**
 * call this function when allocate memory for User pointer
 *
 * @param user the pointer can not be NULL; needed to be allocated to
 */
void mallocUser(User* user);

/**
 * call this function when free User pointer
 * @param user the User pointer needed to be free
 * the pointer can be null
 *
 */
void freeUser(User* user);

/**
 * 根据用户ID来获得用户所有信息
 * @param userId 用户ID
 * @return 如果用户存在, 返回用户所有信息(except the password), 否则返回NULL
 */
User* getUserInfoById(int userId, MYSQL* connection);

/**
 * 查询数据库中是否存在该用户, true和false是宏定义的1和0
 * @param userId 用户ID
 * @return 1: 用户存在数据库中; 0: 用户不存在数据库中; -1: query failed
 */
Status isUserExist(int userId, MYSQL* connection);

/**
 * 注册成功时调用此函数.
 * 将一条user数据插入数据库.
 *
 * must assign:
 * userNickName, userPassword,
 * userSignature(default value is ''),
 * userAvatar(avatar path in server, default value is 'img/defaultAvatar.png),
 * userIp(when user register, default value is 0)
 *
 * needn't assign:
 * userStatus为0, userFriNum为0.
 * 该函数不会对free掉user指针.
 * @param user 要插入的user数据元组的指针
 * @return insert success: userId; insert failure: -1;
 */
int insertUser(User* user, MYSQL* connection);

/**
 * call this function when user change his avatar, nickname or signature.
 * @param userId
 * @param string the string user wants to change to. the end of string must be '\0'
 * @param connection MYSQL connection pointer
 * @param type 1: AVATAR; 2: NICKNAME; 3: SIGNATURE
 * @return 1: update success; -1: update failure
 */
Status updateUserString(int userId, char *string, MYSQL* connection, int type);

/**
 * 当用户上线或下线时调用此函数.
 * @param userId 用户ID
 * @param status 1: 上线, 0: 下线
 * @return 1: 更新数据库成功, -1: 更新数据库失败
 */
Status updateUserStatus(int userId, int status, MYSQL* connection);

/**
 * 当用户网络状态发生改变(包括手动上下线)时, 调用此函数
 * 下线时好像调用与否都不影响. 交由使用者判断是否调用
 * @param userId 用户ID
 * @param userIp 网络状态改变后的ip地址
 * @return 1: 更新数据库成功, -1: 更新数据库失败
 */
Status updateUserIp(int userId, int userIp, MYSQL* connection);

/**
 * 验证密码时调用此函数
 * @param userId 用户ID
 * @param password 要验证的密码
 * @return 1: 密码正确; 0: 密码错误; -1: query failed
 */
Status checkUserPassword(int userId, char *password, MYSQL* connection);

/**
 * add or minus 1 to the userFriNum of the specified user
 * @param userId
 * @param connection MYSQL* connection
 * @param type INCREASE: 1; DECREASE: -1
 * @return 1: update success; -1: update fail
 */
Status updateUserFriendNum(int userId, MYSQL *connection, int type);

#endif //LINPOP_DATABASE_USER_H
