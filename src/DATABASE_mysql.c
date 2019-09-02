//
// Created by new on 9/1/19.
//

//
// Created by new on 9/1/19.
//
#include "../include/DATABASE_mysql.h"
#include <stdio.h>
MYSQL*  mysqlConnection()
{
    MYSQL *connection = (MYSQL*)malloc(sizeof(MYSQL));
    mysql_init(connection);
    if(!mysql_real_connect(connection, HOST, USERNAME, PASSWORD, "linpop", 3306, NULL, 0))
    {
        perror("CONNECTION FAIL\n");
    }
    else
    {
        printf("Connection success\n");
    }
    return connection;
}

void mysqlCloseConnection(MYSQL *connection)
{
    mysql_close(connection);
    printf("Bye");
}
