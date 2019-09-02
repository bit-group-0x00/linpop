/*
  File Name: NET_server.c
  Model Name: NET
  Create Date: 2019/8/29
  Author: PengYao
  Abstract Description: server side program
*/

#include "../include/NET_server.h"

void handle_regist_request(int client, cJSON* cjson)
{
    char* nick_name = cJSON_GetObjectItem(cjson, "nick_name")->valuestring;
    char* passwd = cJSON_GetObjectItem(cjson, "passwd")->valuestring;
    char* avatar = cJSON_GetObjectItem(cjson, "avatar")->valuestring;
    int id;
    User user;
    user.userNickName = nick_name;
    user.userPassword = passwd;
    user.userAvatar = avatar;
    /* 创建response的cjson */
    cJSON* cj = cJSON_CreateObject();
    if((id = insertUser(user)) != -1)
    {
        cJSON_AddItemToObject(cj, "type", cJSON_CreateNumber(SUCCESS));
        cJSON_AddItemToObject(cj, "id", cJSON_CreateNumber(id));
    } else
    {
        cJSON_AddItemToObject(cj, "type", cJSON_CreateNumber(FAILURE));
    }
    send_cjson(client, cj);
    cJSON_Delete(cj);
}

void handle_login_request(int client, cJSON* cjson)
{
    int id = cJSON_GetObjectItem(cjson, "id")->valueint;
    char* passwd = cJSON_GetObjectItem(cjson, "passwd")->valuestring;
    /* 创建response的cjson */
    cJSON* cj = cJSON_CreateObject();
    if(isUserExist(id) && checkUserPassword(id, passwd))
    {
        cJSON_AddItemToObject(cj, "type", cJSON_CreateNumber(SUCCESS));
    } else
    {
        cJSON_AddItemToObject(cj, "type", cJSON_CreateNumber(FAILURE));
    }
    send_cjson(client, cj);
    cJSON_Delete(cj);
}

void handle_msg_send(int client, cJSON* cjson)
{
    int from_id = cJSON_GetObjectItem(cjson, "from_id")->valueint;
    int to_id = cJSON_GetObjectItem(cjson, "target_id")->valueint;
    char* msg = cJSON_GetObjectItem(cjson, "message")->valuestring;
    printf("recieved message [%s] from %s.\n", msg, from_id);
    printf("message will be resent to %s.\n", to_id);
    /* resend write below */
}

void handle_file_send(int clinet, cJSON* cjson)
{

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