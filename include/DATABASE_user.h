//
// Created by junqi on 2019/8/30.
//

#ifndef LINPOP_DATABASE_USER_H
#define LINPOP_DATABASE_USER_H

#include <stdbool.h>
typedef int Status;

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
 * 根据用户ID来获得用户所有信息
 * @param userId 用户ID
 * @return 如果用户存在, 返回用户所有信息, 否则返回NULL
 */
User getUserInfoById(int userId);

/**
 * 查询数据库中是否存在该用户, true和false是宏定义的1和0
 * @param userId 用户ID
 * @return true: 用户存在数据库中 , false: 用户不存在数据库中
 */
Status isUserExist(int userId);

/**
 * 注册成功时调用此函数.
 * 将一条user数据插入数据库. 不需要指定userId.
 * userStatus为0, userFriNum为0.
 * userIp为注册时的IP地址.
 * 该函数不会对free掉user指针.
 * @param user 要插入的user数据元组的指针
 * @return true: 插入数据成功, false: 插入数据失败
 */
Status insertUser(User* user);

/**
 * 当用户的新头像已经上传到服务器后调用
 * @param userId 更改头像的用户ID
 * @param userAvatar 头像文件在服务器存储的路径
 * @return true: 更新数据库成功, false: 更新数据库失败
 */
Status updateUserAvatar(int userId, char *userAvatar);

/**
 * 当用户更改昵称并执行确定操作时调用
 * @param userId 用户ID
 * @param signature 用户要更改后的昵称
 * @return true: 更新数据库成功, false: 更新数据库失败
 */
Status updateUserNickName(int userId, char *signature);

/**
 * 当用户更改个性签名并执行确定操作时调用
 * @param userId 用户ID
 * @param signature 用户要更改后的个性签名
 * @return true: 更新数据库成功, false: 更新数据库失败
 */
Status updateUserSignature(int userId, char *signature);

/**
 * 当用户上线或下线时调用此函数.
 * @param userId 用户ID
 * @param status true: 上线, false: 下线
 * @return true: 更新数据库成功, false: 更新数据库失败
 */
Status updateUserStatus(int userId, Status status);

/**
 * 当用户网络状态发生改变(包括手动上下线)时, 调用此函数
 * 下线时好像调用与否都不影响. 交由使用者判断是否调用
 * @param userId 用户ID
 * @param userIp 网络状态改变后的ip地址
 * @return true: 更新数据库成功, false: 更新数据库失败
 */
Status updateUserIp(int userId, int userIp);

/**
 * 验证密码时调用此函数
 * @param userId 用户ID
 * @param password 要验证的密码
 * @return true: 密码正确, false: 密码错误
 */
Status checkUserPassword(int userId, char *password);


#endif //LINPOP_DATABASE_USER_H
