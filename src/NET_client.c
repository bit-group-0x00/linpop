/*
  File Name: NET_client.c
  Model Name: NET
  Create Date: 2019/8/29
  Author: PengYao
  Abstract Description: clent side program
*/

#include "../include/NET_client.h"

int my_id = -1;

/* 保存服务器的socket */
int server;
/* 用于向服务器读取数据的buffer */
char server_buff[BUFF_SIZE];
/* 指明server_buffer里面的剩余数据量 */
int server_buff_remain = 0;

state regist(const char* nick_name, const char* passwd)
{
    /* create cjson of regist request */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(REGIST));
    cJSON_AddItemToObject(cjson, "nick_name", cJSON_CreateString(nick_name));
    cJSON_AddItemToObject(cjson, "passwd", cJSON_CreateString(passwd));
    /* send cjson */
    send_cjson(server, cjson);
    cJSON_Delete(cjson);
    cjson = recv_cjson(server, server_buff, &server_buff_remain);
    state s =  cJSON_GetObjectItem(cjson, "type")->valueint;
    cJSON_Delete(cjson);
    return s;  
}

state login(const int id, const char* passwd)
{
    /* create cjson of regist request */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(LOGIN));
    cJSON_AddItemToObject(cjson, "id", cJSON_CreateNumber(id));
    cJSON_AddItemToObject(cjson, "passwd", cJSON_CreateString(passwd));
    /* send cjson */
    send_cjson(server, cjson);
    cJSON_Delete(cjson);
    cjson = recv_cjson(server, server_buff, &server_buff_remain);
    state s = cJSON_GetObjectItem(cjson, "type")->valueint;
    if(s == SUCCESS) my_id = id;
    cJSON_Delete(cjson);
    return s;
}

state send_msg_to_friend(const int friend_id, const char* msg, void(*callback)(state))
{
    /* create cjson of send message */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(SEND_MESSAGE));
    cJSON_AddItemToObject(cjson, "from_id", cJSON_CreateNumber(my_id));
    cJSON_AddItemToObject(cjson, "to_id", cJSON_CreateNumber(friend_id));
    cJSON_AddItemToObject(cjson, "msg", cJSON_CreateString(msg));
    /* send cjson */
    send_cjson(server, cjson);
    cJSON_Delete(cjson);
    cjson = recv_cjson(server, server_buff, server_buff_remain);
    state s = cJSON_GetObjectItem(cjson, "type")->valueint;
}

/* 
  client side cjson handle function 
  客户端的cjson处理函数，不需要被外部调用，不用在.h文件中声名。
  实现的功能是解析cjson，并且根具cjson的类型执行相应的命令。
  在调用监听端口（monitor_port）和监听socket（monitor_socket）
  的时候会需要传入这个函数作为参数。
  具体执行的流程转到NET_socket.h头文件里的monitor_port()
  和monitor_socket()这两个函数。
*/
void handle_cjson(int socket, cJSON* cjson)
{
    cJSON* type = cJSON_GetObjectItem(cjson, "type");
    switch(type->valueint)
    {
        
        default:
        {
            printf("recieved unknown type message:\n");
            char* s = cJSON_Print(cjson);
            printf("%s\n", s);
            free(s);
        }
    }
}

int main(int argc, char* argv[])
{
    /* connect to server */
    if((server = conn_to(SERVER_IP, SERVER_PORT)) == -1)
    {
        printf("cannot connect to server, try again later.\n");
        return -1;
    }
    /* test regist, login and send message */
    regist("pengyao", "123456");
    login(11111111, "123456");
    //send_msg(server, "pengyao", "helloworld!");
    close(server);
    return 0;
}