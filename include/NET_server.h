/*
  File Name: NET_server.h
  Model Name: NET
  Create Date: 2019/8/29
  Author: PengYao
  Abstract Description: server side program
*/

#include "NET_macro.h"
#include "NET_socket.h"
#include "UTIL_cJSON.h"
#include "DATABASE_mysql.h"
#include "DATABASE_user.h"
#include "DATABASE_groupUsers.h"
#include "DATABASE_message.h"
#include "DATABASE_groupMessage.h"
#include "DATABASE_group.h"
#include "DATABASE_friends.h"

/* 
  服务端的cjson解析函数，通过对cjson的类型（type）进行解析，
  从而调用不同的handle。
 */
void handle_cjson(int client, cJSON* cjson);

/* server entrance */
int main(int argc, char* argv[]);