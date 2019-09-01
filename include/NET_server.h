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

/* call after recieving regist request */
void handle_regist_request(int client, cJSON* cjson);

/* call after recieving login request */
void handle_login_request(int client, cJSON* cjson);

/* call after recieving message from client. resend it and write to datebase */
void handle_msg(int client, cJSON* cjson);

/* server side cjson handle function */
void handle_cjson(int socket, cJSON* cjson);

/* server entrance */
int main(int argc, char* argv[]);