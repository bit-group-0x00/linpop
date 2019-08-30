/*
  File Name: NET_client.h
  Model Name: NET
  Create Date: 2019/8/29
  Author: PengYao
  Abstract Description: clent side program
*/

#include "NET_macro.h"
#include "NET_socket.h"
#include "UTIL_cJSON.h"

/* client regist fuction */
void regist(int server, const char* account, const char* passwd);

/* client login function */
void login(int server, const char* account, const char* passwd);

/* client send message function, send to another client or server.
if send to another client append your account, if send to server append target client account */
void send_msg(int server, const char* account, const char* msg);

/* call after regist success */
void regist_success_handle(char* account);

/* call after regist failure */
void regist_failure_handle(char* account);

/* call after lgoin success */
void login_success_handle(char* account);

/* call after login failure */
void login_failure_handle(char* account);

// /* client listen message fuction, perform action when recieve message */
// void* listen_msg(void* socket);

/* client handle message function, if you want to add new action type, 
define it's macro and modifly it */
void* handle_msg(void* socket);

/* client entrance */
int main(int argc, char* argv[]);