/*
  File Name: NET_socket.c
  Model Name: NET
  Create Date: 2019/8/29
  Author: PengYao
  Abstract Description: my socket
*/

#include "../include/NET_socket.h"

//char buffer[BUFF_SIZE];

/* get path */
void get_path(char buff[], char* name);

/* get file size */
int get_file_size(char* file_path);

/* get file name */
char* get_file_name(char* file_path);

void get_path(char buff[], char* name)
{
    if(name[0] == '/') strcpy(buff, name);
    else
    {
        char* home = getenv("HOME");
        int pos = 0;
        while(home[pos] != '\0') buff[pos++] = home[pos];
        strcpy(buff + pos, "/linpop/");
        /* if directory is not exist, create one */
        if(access(buff, 0755) != 0)
            mkdir(buff, 0755);
        strcat(buff, "file/");
        if(access(buff, 0755) != 0)
            mkdir(buff, 0755);
        strcat(buff, name);
    }
}

int get_file_size(char* file_path)
{
    struct stat stat_buf;
    stat(file_path, &stat_buf);
    return stat_buf.st_size;
}

char* get_file_name(char* file_path)
{
    char *file_name = file_path, *temp = file_path;
    while(*temp != '\0')
    {
        if(*temp == '/') file_name = temp;
        ++temp;
    }
    return ++file_name;
}

int conn_to(int ip, int port)
{
    int sckt;
    struct sockaddr_in addr;
    if((sckt = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = ip;
    if(connect(sckt, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        return -1;
    }
    return sckt;
}

void* monitor_socket(void* arg)
{
    pthread_detach(pthread_self());
    int socket = ((struct args*)arg)->value, buff_remain = 0;
    void (*handle)(int, cJSON*) = ((struct args*)arg)->handle;
    char buff[BUFF_SIZE];
    while(1)
    {
        cJSON* cjson = recv_cjson(socket, buff, &buff_remain);
        if(cjson == NULL) break;
        handle(socket, cjson);
        cJSON_Delete(cjson);
    }
}

void* monitor_port(void* arg)
{
    pthread_detach(pthread_self());
    int server, client, addr_len = sizeof(struct sockaddr_in);
    int port = ((struct args*)arg)->value;
    struct sockaddr_in server_addr, client_addr;
    if((server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 1;
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
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
    printf("start listen message at port %d\n", port);
    while(1)
    {
        client = accept(server, (struct sockaddr*)&client_addr, (socklen_t*)&addr_len);
        if(client < 0)
        {
            perror("accept");
            continue;
        }
        printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
        pthread_t thread;
        ((struct args*)arg)->value = client;
        if(pthread_create(&thread, NULL, monitor_socket, (void*)arg) != 0)
        {
            perror("thread create failed");
            continue;
        }
    }
    close(server);
}

state send_cjson(int socket, cJSON* cjson)
{
    char* s = cJSON_PrintUnformatted(cjson);
    /* using '\0' to split different packet, so the len should +1 */
    int len = strlen(s) + 1;
    if(len > BUFF_SIZE)
    {
        perror("buffer overflow");
        return -1;
    }
    send(socket, s, len, 0);
    //printf("send: %s\n", s);
    free(s);
    return 0;
}

cJSON* recv_cjson(int socket, char* buff, int* buff_remain)
{
    /* the pos of spliter '\0' */
    int pos = 0, remain = buff_remain == NULL ? 0 : *buff_remain;
    if(buff == NULL)
    {
        char temp[BUFF_SIZE];
        buff = temp;
    }
    while(pos < remain && buff[pos] != '\0') ++pos;
    if(pos == remain)
    {
        int recv_len = recv(socket, buff + remain, BUFF_SIZE - remain, 0);
        if(recv_len < 0)
        {
            perror("recieve null");
            return NULL;
        }
        else if(recv_len == 0)
        {
            perror("disconnected");
            return NULL;
        }
        remain += recv_len;
        while(buff[pos] != '\0') ++pos;
    }
    //printf("recieved: %s\n", buff);
    cJSON* cjson =  cJSON_Parse(buff);
    remain -= pos + 1;
    for(int i = 0; i < remain; ++i)
    {
        buff[i] = buff[i + pos + 1];
    }
    if(buff_remain != NULL) *buff_remain = remain;
    return cjson;
}

state send_file(int socket, const char* file_path, int size, void(*callback)(state))
{
    char buff[BUFF_SIZE];
    get_path(buff, file_path);
    FILE* file = fopen(buff, "r");
    if(file == NULL)
    {
        perror("can't open file");
        return ERROR;
    }
    int remain = size, progress = 0, new_progress = 0;
    while(remain > 0)
    {
        int read_len = fread(buff, sizeof(char), BUFF_SIZE, file);
        send(socket, buff, read_len, 0);
        remain -= read_len, new_progress = (int)(100.0 * (size - remain) / size);
        if(callback != NULL && new_progress != progress) callback(progress = new_progress);
    }
    if(callback != NULL) callback(SUCCESS);
    fclose(file);
    return SUCCESS;
}

state recv_file(int socket, int size, const char* file_path)
{
    char buff[BUFF_SIZE];
    get_path(buff, file_path);
    FILE* file = fopen(buff, "w");
    if(file == NULL)
    {
        perror("can't create file");
        return ERROR;
    }
    while(size > 0)
    {
        int recv_len = recv(socket, buff, BUFF_SIZE, 0);
        fwrite(buff, sizeof(char), recv_len, file);
        size -= recv_len;
    }
    fclose(file);
    return SUCCESS;
}

// ip get_my_ip()
// {
//     struct ifaddrs* ifaddr = NULL;
//     void* addr_ptr = NULL;
//     if(getifaddrs(&ifaddr) != 0)
//     {
//         perror("get ip address error");
//         return -1;
//     }
//     struct ifaddrs* iter = ifaddr;
//     while(iter != NULL)
//     {
//         /* ipv4 */
//         if(iter->ifa_addr->sa_family == AF_INET)
//         {
//             //printf("my ip: %s\n", inet_ntoa(((struct sockaddr_in*)iter->ifa_addr)->sin_addr));
//             if(strcmp(inet_ntoa(((struct sockaddr_in*)iter->ifa_addr)->sin_addr), "127.0.0.1") != 0)
//             return ((struct sockaddr_in*)iter->ifa_addr)->sin_addr.s_addr;
//         }
//         iter = iter->ifa_next;
//     }
//     perror("get ip error");
//     return -1;
// }

int get_ip(int socket)
{
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    if(getpeername(socket, (struct sockaddr*)&addr, &addrlen) == -1)
    {
        perror("can't get socket ip");
        return ERROR;
    }
    return addr.sin_addr.s_addr;
}