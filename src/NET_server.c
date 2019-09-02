/*
  File Name: NET_server.c
  Model Name: NET
  Create Date: 2019/8/29
  Author: PengYao
  Abstract Description: server side program
*/

#include "../include/NET_server.h"

/*
  回复操作成功
*/
void response_state(int client, state type);

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
  同上，在接受到登出请求后会自动调用该函数
*/
void handle_logout_request(int client, cJSON* cjson);

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


void response_state(int client, state type)
{
    cJSON* response = cJSON_CreateObject();
    cJSON_AddItemToObject(response, "type", cJSON_CreateNumber(type));
    send_cjson(client, response);
    cJSON_Delete(response);
}

void handle_regist_request(int client, cJSON* cjson)
{
    int id;
    user.userNickName = cJSON_GetObjectItem(cjson, "nick_name")->valuestring;
    user.userPassword = cJSON_GetObjectItem(cjson, "passwd")->valuestring;
    user.userAvatar = cJSON_GetObjectItem(cjson, "avatar")->valuestring;
    state type = FAILURE;
    if((id = insertUser(user)) != -1) type = SUCCESS;
    /* 创建response的cjson */
    cJSON* response = cJSON_CreateObject();
    cJSON_AddItemToObject(response, "type", cJSON_CreateNumber(type));
    cJSON_AddItemToObject(response, "id", cJSON_CreateNumber(id));
    send_cjson(client, response);
    cJSON_Delete(response);
}

void handle_login_request(int client, cJSON* cjson)
{
    int id = cJSON_GetObjectItem(cjson, "id")->valueint;
    char* passwd = cJSON_GetObjectItem(cjson, "passwd")->valuestring;
    state type = FAILURE;
    if(isUserExist(id) && checkUserPassword(id, passwd) && updateUserStatus(id, \
    1, conn) && updateUserIp(id, ip, conn)) type = SUCCESS;
    response_state(client, type);
}

void handle_logout_request(int client, cJSON* cjson)
{
    int id = cJSON_GetObjectItem(cjson, "id")->valueint;
    state type = FAILURE;
    if(updateUserStatus(ip, 0, conn)) type = SUCCESS;
    response_state(client, type);
}

void handle_msg_send(int client, cJSON* cjson)
{
    Message msg;
    msg.msgFromId = cJSON_GetObjectItem(cjson, "origin")->valueint;
    msg.msgToId = cJSON_GetObjectItem(cjson, "target")->valueint;
    msg.msgContent = cJSON_GetObjectItem(cjson, "message")->valuestring;
    msg.msgStatus = 0;
    User user = getUserInfoById(target);
    /* 如果对方在线 */
    if(user.userStatus == 1)
    {
        int client_2 = conn_to(user.userIP, CLIENT_PORT);
        send_cjson(client_2, cjson);
        cJSON* cj_2 = recv_cjson(client_2, NULL, NULL);
        if(cJSON != NULL)
        {
            msg.msgStatus = 1;
            cJSON_Delete(cj_2);
        }
    }
    insertMsg(msg, conn);
    mallocUser(user);
    response_state(client, type);
}

void handle_file_send(int clinet, cJSON* cjson)
{
    Message msg;
    msg.msgFromId = cJSON_GetObjectItem(cjson, "origin")->valueint;;
    msg.msgToId = cJSON_GetObjectItem(cjson, "target")->valueint;
    msg.msgContent = cJSON_GetObjectItem(cjson, "name")->valuestring;
    msg.msgStatus = 0;
    int size = cJSON_GetObjectItem(cjson, "size")->valueint;
    char* name = msg.msgContent;
    state type = recv_file(client, size, name);
    /* send to server */
    if(target == 0) response_state(type);
    else if(type == SUCCESS)
    {
        User user = getUserInfoById(target);
        if(user.userStatus == 1)
        {
            int client_2 = conn_to(user.userIP, CLIENT_PORT);
            send_cjson(client_2, cjson);
            cJSON* cj_2 = recv_cjson(client_2, NULL, NULL);
            if(cj_2 != NULL)
            {
                send_file(client_2, name, NULL);
                cJSON_Delete(cj_2);
            }
            msg.msgStatus = 1;
        }
        mallocUser(user);
    }
    insertMsg(msg, conn);
}

void handle_cjson(int client, cJSON* cjson)
{
    cJSON* type = cJSON_GetObjectItem(cjson, "type");
    void (*handle)(int, cJSON*);
    switch(type->valueint)
    {
        case REGIST: handle = handle_regist_request; break;
        case LOGIN: handle = handle_regist_request; break;
        case SEND_MESSAGE: handle = handle_msg_send; break;
        case SEND_FILE: handle = handle_file_send; break;
        default:
        {
            printf("recieved unknown type message:\n");
            char* s = cJSON_Print(cjson);
            printf("%s\n", s);
            free(s);
            return;
        }
    }
    handle(client, cjson);
}

int main(int argc, char* argv[])
{
    int retry = 5;
    struct args arg;
    arg.value = SERVER_PORT, arg.handle = handle_cjson;
    while(retry--)
    {
        monitor_port((void*)&arg);
        printf("error occurred, retry...\n");
    }
    printf("network error, try again later\n");
    return 0;
}