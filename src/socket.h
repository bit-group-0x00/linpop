/*
  my socket
*/
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "macro.h"
#include "util/cJSON.h"

#define BUFF_SIZE 2048
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6789
#define CLIENT_PORT 9876
extern char buffer[];
/* establish a connection to ip */
int con_to(const char* ip, int port);
/* send message to socket */
void send_cjson(int socket, cJSON* msg);
/* recieve message from socket */
cJSON* recv_cjson(int socket);
