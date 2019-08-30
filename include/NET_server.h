/*
  server side program
*/
#include "NET_macro.h"
#include "NET_socket.h"
#include "../src/util/cJSON.h"
void regist(int client, const char* account, const char* passwd);
void login(int client, const char* account, const char* passwd);
int listen_msg();
void handle_msg(int client, cJSON* msg);
int main(int argc, char* argv[]);