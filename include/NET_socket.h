/*
  File Name: NET_socket.h
  Model Name: NET
  Create Date: 2019/8/29
  Author: PengYao
  Abstract Description: my socket
*/

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "NET_macro.h"
#include "UTIL_cJSON.h"

#define DEFAULT_SERVER_IP "10.194.59.79"
#define SERVER_PORT 6789
#define CLIENT_PORT 9876
#define BUFF_SIZE 10240

struct args
{
    int value;
    void (*handle)(int, cJSON*);
};

/* 
  establish a connection to ip:port 
  :param ip: ip
  :param port: port
  :return: socket
*/
int conn_to(int ip, int port);

/* 
  socket monitor 
  :param arg: struct args
*/
void* monitor_socket(void* arg);

/* 
  port monitor 
  :param arg: struct args
*/
void* monitor_port(void* arg);

/* 
  send cjson to socket 
  :param socket: socket
  :param cjson: cjson to be sent
  :return: 0(successful) or 1(failed)
*/
state send_cjson(int socket, cJSON* cjson);

/* 
  recieve cjson from socket
  :param socket: socket
  :param buffer: buffer
  :param buff_remain: remain data in buffer
  :return: cjson
*/
cJSON* recv_cjson(int socket, char* buff, int* buff_remain);

/*
  send file to socket
  发送file_path指定的文件到目标socket
  返回值为：SUCCESS（0）正在发送，FAILURE（1）发送失败
  ERROR（-1）发生错误。
  在发送的过程中会根据进度调用callback函数刷新界面，
  callback函数的的参数具体含义如下：
  SUCCESS（0）表示发送完成，ERROR（-1）表示发送失败，
  E1～100表示进度值
  注：如果传入的callback为空就不调用该函数。
*/
state send_file(int socket, const char* file_path, int size, void(*callback)(state));

/*
  recieve file from socket
  :param socket: socket
  :param buff: buffer
  :param name: file name
  :return: saved file
*/
state recv_file(int socket, int size, const char* name);

/*
  get my ip address
  :return: (unsigned int)ip
*/
// ip get_my_ip();

/* get ip by socket */
int get_ip(int socket);

/* get aip by socket */
char* get_aip(int socket);

/*
  回复操作结果
*/
void response_state(int client, state type);

/* */
char* get_aip_2(int ip);