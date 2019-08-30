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
    printf("sorry. your account %s regist failed.\n", account);
}
void listen_msg(int server)
{
    while(1)
    {
        cJSON* msg = recv_cjson(server);
        if(msg == NULL)
        {
            perror("recieve null");
            break;
        }
        handle_msg(msg);
        cJSON_Delete(msg);
    }
}
void handle_msg(cJSON* msg)
{
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
    /* test regist and login */
    printf("regist request with account = \"pengyao\", passwd = \"123456\"\n");
    regist(server, "pengyao", "123456");
    printf("login request with account = \"pengyao\", passwd = \"123456\"\n");
    login(server, "pengyao", "123456");
    listen_msg(server);
    return 0;
}