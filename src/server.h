/*
  server side program
*/
#include "macro.h"
#include "socket.h"
#include "util/cJSON.h"
void regist(int client, const char* account, const char* passwd);
void login(int client, const char* account, const char* passwd);
int listen_msg();
void handle_msg(int client, cJSON* msg);
int main(int argc, char* argv[]);