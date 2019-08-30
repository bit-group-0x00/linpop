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

/* server regist function */
void regist(int client, const char* account, const char* passwd);

/* server login function */
void login(int client, const char* account, const char* passwd);

/* call after recieved message from client. resend it and write to datebase */
void recv_msg_handle(int client, const char* from, const char* to, const char* msg);

/* server listen message function, listen message from client */
int listen_msg();

/* server handle message function, if you want to add new action type, 
define it's macro and modifly it */
void* handle_msg(void* socket);

/* server entrance */
int main(int argc, char* argv[]);