//
// Created by junqi on 2019/8/30.
//

#ifndef LINPOP_DATABASE_MYSQL_H
#define LINPOP_DATABASE_MYSQL_H

#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>

#define HOST "127.0.0.1"
#define USERNAME "linpop"
#define PASSWORD "linpop"
#define DATABASE "linpop"
#define PORT 3306

#define INCREASE 1
#define DECREASE -1
#define MESSAGE_MAX_LENGTH 1000
#define DATETIME_LENGTH 100
#define SQL_SELECT_LAST_ID "SELECT LAST_INSERT_ID();"
#define SQL_LENGTH_MAX 400
typedef int Status;

/**
 * 连接数据库
 * @return 1:MYSQL(提示success)   -1:NULL MYSQL(提示fail)
 */
MYSQL*  mysqlConnection();

/**
 * 关闭数据库
 * @param connection 数据库连接句柄
 */
void mysqlCloseConnection(MYSQL *connection);
#endif //LINPOP_DATABASE_MYSQL_H