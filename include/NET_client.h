/*
  client side program
*/
#include "NET_macro.h"
#include "NET_socket.h"
#include "UTIL_cJSON.h"
#include "UI_login.h"
void regist(int server, const char* account, const char* passwd);
void login(int server, const char* account, const char* passwd);
void regist_success_handle(char* account);
void regist_failure_handle(char* account);
void login_success_handle(char* account);
void login_failure_handle(char* account);
void listen_msg();
void handle_msg(cJSON* msg);
int main(int argc, char* argv[]);