#include "socket.h"
char buffer[BUFF_SIZE];
int con_to(const char* ip, int port)
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
    addr.sin_addr.s_addr = inet_addr(ip);
    if(connect(sckt, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        return -1;
    }
    return sckt;
}
void send_cjson(int socket, cJSON* cjson)
{
    char* s = cJSON_Print(cjson);
    send(socket, s, strlen(s), 0);
    free(s);
}
cJSON* recv_cjson(int socket)
{
    int recv_len = recv(socket, buffer, BUFF_SIZE, 0);
    if(recv_len < 0)
    {
        perror("recieve null");
        return NULL;
    }
    buffer[recv_len] = '\0';
    return cJSON_Parse(buffer);
}