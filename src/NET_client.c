/*
  File Name: NET_client.c
  Model Name: NET
  Create Date: 2019/8/29
  Author: PengYao
  Abstract Description: clent side program
*/

#include "../include/NET_client.h"

void regist(int server, const char* account, const char* passwd)
{
    /* create cjson of regist request */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(REGIST_REQUEST));
    cJSON_AddItemToObject(cjson, "account", cJSON_CreateString(account));
    cJSON_AddItemToObject(cjson, "passwd", cJSON_CreateString(passwd));
    /* send cjson */
    send_cjson(server, cjson);
    cJSON_Delete(cjson);
}

void login(int server, const char* account, const char* passwd)
{
    /* create cjson of regist request */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(LOGIN_REQUEST));
    cJSON_AddItemToObject(cjson, "account", cJSON_CreateString(account));
    cJSON_AddItemToObject(cjson, "passwd", cJSON_CreateString(passwd));
    /* send cjson */
    send_cjson(server, cjson);
    cJSON_Delete(cjson);
}

void send_msg(int server,const char* account, const char* msg)
{
    /* create cjson of send message */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(SEND_MESSAGE));
    cJSON_AddItemToObject(cjson, "from", cJSON_CreateString(account));
    cJSON_AddItemToObject(cjson, "to", cJSON_CreateString(account));
    cJSON_AddItemToObject(cjson, "message", cJSON_CreateString(msg));
    /* send cjson */
    send_cjson(server, cjson);
    cJSON_Delete(cjson);
}

void regist_success_handle(char* account)
{
    /* called after regist successful */
    printf("congratulation! your account %s regist successful.\n", account);
}

void regist_failure_handle(char* account)
{
    /* called after regist failed */
    printf("sorry. your account %s regist failed.\n", account);
}

void login_success_handle(char* account)
{
    /* called after login successful */
    printf("congratulation! your account %s login successful.\n", account);
}

void login_failure_handle(char* account)
{
    /* call after login failed */
    printf("sorry. your account %s login failed.\n", account);
}

// void* listen_msg(void* server)
// {
//     while(1)
//     {
//         cJSON* msg = recv_cjson(server);
//         if(msg == NULL)
//         {
//             perror("recieve null");
//             break;
//         }
//         handle_msg(msg);
//         cJSON_Delete(msg);
//     }
// }

void* handle_msg(void* socket)
{
    int sckt = *((int*)socket);
    while(1)
    {
        cJSON* msg = recv_cjson(sckt);
        if(msg == NULL)
        {
            perror("recieve null");
            break;
        }
        cJSON* type = cJSON_GetObjectItem(msg, "type");
        switch(type->valueint)
        {
            case REGIST_SUCCESS:
            {
                char* account = cJSON_GetObjectItem(msg, "account")->valuestring;
                regist_success_handle(account);
                break;
            }
            case REGIST_FAILURE:
            {
                char* account = cJSON_GetObjectItem(msg, "account")->valuestring;
                regist_failure_handle(account);
                break;
            }
            case LOGIN_SUCCESS:
            {
                char* account = cJSON_GetObjectItem(msg, "account")->valuestring;
                login_success_handle(account);
                break;
            }
            case LOGIN_FAILURE:
            {
                char* account = cJSON_GetObjectItem(msg, "account")->valuestring;
                login_failure_handle(account);
                break;
            }
            default:
            {
                printf("recieved unknown type message:\n");
                char* s = cJSON_Print(msg);
                printf("%s\n", s);
                free(s);
            }
        }
        cJSON_Delete(msg);
    }
    pthread_exit("disconnected");
}

int main(int argc, char* argv[])
{
    /* connect to server */
    int retry = 5, server = -1;
    while(retry-- > 0)
    {
        /* connect to server */
        if((server = con_to(SERVER_IP, SERVER_PORT)) != -1)
        {
            break;
        }
    }
    if(server == -1)
    {
        printf("cannot connect to server, try again later.\n");
        return -1;
    }
    pthread_t thread;
    if(pthread_create(&thread, NULL, handle_msg, (void*)&server) != 0)
    {
        perror("thread create failed");
        return -1;
    }
    /* test regist, login and send message */
    printf("regist request with account = \"pengyao\", passwd = \"123456\"\n");
    regist(server, "pengyao", "123456");
    printf("login request with account = \"pengyao\", passwd = \"123456\"\n");
    login(server, "pengyao", "123456");
    printf("sending message to server.\n");
    send_msg(server, "pengyao", "helloworld!");
    while(1)
    {

    }
    close(server);
    return 0;
}