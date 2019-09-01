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
    printf("registing for %s\n", nick_name);
    printf("regist failed.\n");
    /* create cjson of regist failure */
    cJSON* cj = cJSON_CreateObject();
    cJSON_AddItemToObject(cj, "type", cJSON_CreateNumber(FAILURE));
    //cJSON_AddItemToObject(cjson, "account", cJSON_CreateString(account));
    //cJSON_AddItemToObject(cjson, "passwd", cJSON_CreateString(passwd));
    /* send cjson */
    send_cjson(client, cj);
    cJSON_Delete(cj);
}

void handle_login_request(int client, cJSON* cjson)
{
    int id = cJSON_GetObjectItem(cjson, "id")->valueint;
    char* passwd = cJSON_GetObjectItem(cjson, "passwd")->valuestring;
    printf("logging in for %d\n", id);
    printf("login failed.\n");
    /* create cjson of login failure */
    cJSON* cj = cJSON_CreateObject();
    cJSON_AddItemToObject(cj, "type", cJSON_CreateNumber(FAILURE));
    //cJSON_AddItemToObject(cjson, "account", cJSON_CreateString(account));
    //cJSON_AddItemToObject(cjson, "passwd", cJSON_CreateString(passwd));
    /* send cjson */
    send_cjson(client, cj);
    cJSON_Delete(cj);
}

void handle_msg(int client, cJSON* cjson)
{
    int from_id = cJSON_GetObjectItem(cjson, "from_id")->valueint;
    int to_id = cJSON_GetObjectItem(cjson, "target_id")->valueint;
    char* msg = cJSON_GetObjectItem(cjson, "message")->valuestring;
    printf("recieved message [%s] from %s.\n", msg, from_id);
    printf("message will be resent to %s.\n", to_id);
    /* resend write below */
}

void handle_cjson(int socket, cJSON* cjson)
{
    cJSON* type = cJSON_GetObjectItem(cjson, "type");
    switch(type->valueint)
    {
        case REGIST:
        {
            handle_regist_request(socket, cjson);
            break;
        }
        case LOGIN:
        {
            handle_login_request(socket, cjson);
            break;
        }
        case SEND_MESSAGE:
        {
            handle_msg(socket, cjson);
            break;
        }
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