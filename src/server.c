#include "server.h"
void regist(int client, const char* account, const char* passwd)
{
    printf("registing for %s\n", account);
    /* create cjson of regist failure */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(REGIST_FAILURE));
    cJSON_AddItemToObject(cjson, "account", cJSON_CreateString(account));
    cJSON_AddItemToObject(cjson, "passwd", cJSON_CreateString(passwd));
    /* send cjson */
    send_cjson(client, cjson);
    cJSON_Delete(cjson);
}
void login(int client, const char* account, const char* passwd)
{
    printf("logging in for %s\n", account);
    /* create cjson of login failure */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(LOGIN_FAILURE));
    cJSON_AddItemToObject(cjson, "account", cJSON_CreateString(account));
    cJSON_AddItemToObject(cjson, "passwd", cJSON_CreateString(passwd));
    /* send cjson */
    send_cjson(client, cjson);
    cJSON_Delete(cjson);
}
int listen_msg()
{
    int server, client, addr_len = sizeof(struct sockaddr_in);
    struct sockaddr_in server_addr, client_addr;
    if((server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 1;
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(server, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        return 1;
    }
    if(listen(server, 5) < 0)
    {
        perror("listen");
        return 1;
    }
    printf("start listen message at port %d\n", SERVER_PORT);
    while(1)
    {
        client = accept(server, (struct sockaddr*)&client_addr, (socklen_t*)&addr_len);
        if(client < 0)
        {
            perror("accept");
            continue;
        }
        printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
        /* multi-thread below... */
        while(1)
        {
            cJSON* msg = recv_cjson(client);
            if(msg == NULL)
            {
                perror("recieve null");
                break;
            }
            handle_msg(client, msg);
            cJSON_Delete(msg);
            
        }
    }
    close(server);
}
void handle_msg(int client, cJSON* msg)
{
    cJSON* type = cJSON_GetObjectItem(msg, "type");
    switch(type->valueint)
    {
        case REGIST_REQUEST:
        {
            char* account = cJSON_GetObjectItem(msg, "account")->valuestring;
            char* passwd = cJSON_GetObjectItem(msg, "passwd")->valuestring;
            regist(client, account, passwd);
            break;
        }
        case LOGIN_REQUEST:
        {
            char* account = cJSON_GetObjectItem(msg, "account")->valuestring;
            char* passwd = cJSON_GetObjectItem(msg, "passwd")->valuestring;
            login(client, account, passwd);
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
    int retry = 5;
    while(retry--)
    {
        listen_msg();
        printf("error occurred, retry...\n");
    }
    printf("network error, try again later\n");
    return 0;
}