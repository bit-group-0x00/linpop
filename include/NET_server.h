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
#include "DATABASE_user.h"

/* 
  服务端的handle函数，在接受到注册请求后会自动调用该函数
  函数的参数三请求方的socket和接受到的cjson
*/
void handle_regist_request(int client, cJSON* cjson);

/*
  同上，在接受到登陆请求后会自动调用该函数
*/
void handle_login_request(int client, cJSON* cjson);

/* 
  同上，在接受到发送消息请求后自动调用该函数，其中发送消息
  可以分为两种：用户向其他用户发送消息、用户向群组发送消息
*/
void handle_msg_send(int client, cJSON* cjson);

/* 
  同上，在接受到发送文件请求后自动调用该函数，其中发送文件也可以分为两种
  一种是客户端向服务端发送文件，另一种是客户端向其他客户端发送文件
*/
void handle_file_send(int client, cJSON* cjson);

/* 
  服务端的cjson解析函数，通过对cjson的类型（type）进行解析，
  从而调用不同的handle。
 */
void handle_cjson(int client, cJSON* cjson);

/* server entrance */
int main(int argc, char* argv[]);