/*
  File Name: NET_client.c
  Model Name: NET
  Create Date: 2019/8/29
  Author: PengYao
  Abstract Description: clent side program
*/

#include "../include/NET_client.h"

/* 全局变量my_info */
info my_info;
/* 保存服务器的socket */
int server;
/* 用于向服务器读取数据的buffer */
char server_buff[BUFF_SIZE];
/* 指明server_buffer里面的剩余数据量 */
int server_buff_remain = 0;

/* 
  client side cjson handle function 
  客户端的cjson处理函数，不需要被外部调用，不用在.h文件中声名。
  实现的功能是解析cjson，并且根具cjson的类型执行相应的命令。
  在调用监听端口（monitor_port）和监听socket（monitor_socket）
  的时候会需要传入这个函数作为参数。
  具体执行的流程转到NET_socket.h头文件里的monitor_port()
  和monitor_socket()这两个函数。
*/
void handle_cjson(int socket, cJSON* cjson);

/*
  request my info
  请求自己的详细信息，传入存放详细信息的结构体指针，返回值表示
  请求的结果SUCCESS（0）表示成功，FAILURE（1）表示失败，
  ERROR（-1）表示请求过程中发生了错误。最后的用户数据放在my_info
  指向的结构体里面。
*/
state request_my_info();

// /* parse profile fuction */
// profile parse_profile(cJSON* cjson);

// /* get all my friends function */
// friend* get_friends();

// /* get message function */
// message get_message(cJSON* cjson);

// friend* get_friends()
// {
//     friend fri;
//     fri.friend_profile = parse_profile(cJSON_GetObjectItem(cjson, "friend_profile"));
//     fri.msg = parse_message(cJSON_GetObjectItem(cjson, "message"));
//     return fri;
// }

// profile parse_profile(cJSON* cjson)
// {
//     profile p;
//     p.id = cJSON_GetObjectItem(cjson, "id");
//     char* nick_name = cJSON_GetObjectItem(cjson, "nick_name");
//     p.nick_name = malloc(sizeof(char) * (strlen(nick_name) + 1));
//     strcpy(p.nick_name, nick_name);
//     char* avatar = cJSON_GetObjectItem(cjson, "avatar");
//     p.avatar = malloc(sizeof(char) * (strlen(avatar) + 1));
//     strcpy(p.avatar, avatar);
//     return p;
// }


state regist(const char* nick_name, const char* passwd)
{
    /* create cjson of regist request */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(REGIST));
    cJSON_AddItemToObject(cjson, "nick_name", cJSON_CreateString(nick_name));
    cJSON_AddItemToObject(cjson, "passwd", cJSON_CreateString(passwd));
    cJSON_AddItemToObject(cjson, "avatar", cJSON_CreateString(passwd));
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
    if(s == SUCCESS)
    {
        my_info.my_profile.id = id;
        my_info.my_profile.nick_name = cJSON_GetObjectItem(cjson, "nick_name");
        my_info.my_profile.avatar = cJSON_GetObjectItem(cjson, "avatar");
        s = request_my_info();
    }
    cJSON_Delete(cjson);
    return s;
}

state request_my_info()
{
    /* create cjson of my info request */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(REQUEST_FRIEND_LIST));
    cJSON_AddItemToObject(cjson, "id", cJSON_CreateNumber(my_info.my_profile.id));
    /* todo: here needs more varify information */
    /* send cjson */
    send_cjson(server, cjson);
    cJSON_Delete(cjson);
    cJSON* friend_list_cjson = recv_cjson(server, server_buff, &server_buff_remain);
    //printf("%s\n", cJSON_Print(friend_list_cjson));
    state s = cJSON_GetObjectItem(friend_list_cjson, "type")->valueint;
    if(s == SUCCESS)
    {
        my_info.friend_num = cJSON_GetObjectItem(friend_list_cjson, "friend_num")->valueint;
        my_info.friends = malloc(my_info.friend_num * sizeof(friend));
        cJSON* friends = cJSON_GetObjectItem(friend_list_cjson, "friends");
        cJSON* nick_names = cJSON_GetObjectItem(friend_list_cjson, "nick_names");
        cJSON* avatars = cJSON_GetObjectItem(friend_list_cjson, "avatars");
        for(int i = 0; i < my_info.friend_num; ++i)
        {
            my_info.friends[i].friend_profile.id = cJSON_GetArrayItem(friends, i)->valueint;
            char* nick_name = cJSON_GetArrayItem(nick_names, i)->valuestring;
            my_info.friends[i].friend_profile.nick_name = malloc(sizeof(char) * (strlen(nick_name) + 1));
            strcpy(my_info.friends[i].friend_profile.nick_name, nick_name);
            char* avatar = cJSON_GetArrayItem(avatars, i)->valuestring;
            my_info.friends[i].friend_profile.avatar = malloc(sizeof(char) * (strlen(avatar) + 1));
            strcpy(my_info.friends[i].friend_profile.avatar, avatar);
            my_info.friends[i].first_msg = my_info.friends[i].last_msg = NULL;
            cJSON* request_msg_list = cJSON_CreateObject();
            cJSON_AddItemToObject(request_msg_list, "type", cJSON_CreateNumber(REQUEST_MESSAGE_LIST));
            cJSON_AddItemToObject(request_msg_list, "id", cJSON_CreateNumber(my_info.my_profile.id));
            cJSON_AddItemToObject(request_msg_list, "friend_id", cJSON_CreateNumber(my_info.friends[i].friend_profile.id));
            send_cjson(server, request_msg_list);
            cJSON_Delete(request_msg_list);
            cJSON* msg_list_cjson = recv_cjson(server, server_buff, &server_buff_remain);
            printf("%s", cJSON_Print(msg_list_cjson));
            int msg_num = cJSON_GetObjectItem(msg_list_cjson, "message_num");
            cJSON* contents = cJSON_GetObjectItem(msg_list_cjson, "contents");
            cJSON* senders = cJSON_GetObjectItem(msg_list_cjson, "senders");
            cJSON* dates = cJSON_GetObjectItem(msg_list_cjson, "dates");
            cJSON* states = cJSON_GetObjectItem(msg_list_cjson, "states");
            for(int j = 0; j < msg_num; ++j)
            {
                message* msg = malloc(sizeof(message));
                msg->sender = cJSON_GetArrayItem(senders, i)->valueint;
                msg->checked = cJSON_GetArrayItem(states, i)->valueint;
                char* content = cJSON_GetArrayItem(contents, i)->valuestring;
                msg->content = malloc(sizeof(char) * (strlen(content) + 1));
                strcpy(msg->content, content);
                char* date = cJSON_GetArrayItem(dates, i)->valuestring;
                msg->date = malloc(sizeof(char) * (strlen(date) + 1));
                strcpy(msg->date, date);
                if(my_info.friends[i].last_msg == NULL)
                {
                    my_info.friends[i].last_msg = msg;
                    my_info.friends[i].first_msg = msg;
                }
                else
                {
                    /* add new message to the end of list */
                    my_info.friends[i].last_msg->next = msg;
                    msg->last = my_info.friends[i].last_msg;
                    my_info.friends[i].last_msg = msg;
                }
            }
            cJSON_Delete(msg_list_cjson);
        }
    }
    cJSON_Delete(cjson);
    return s;
}

state send_msg_to_friend(const int friend_id, const char* msg)
{
    /* create cjson of send message */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(SEND_MESSAGE));
    cJSON_AddItemToObject(cjson, "origin", cJSON_CreateNumber(my_info.my_profile.id));
    cJSON_AddItemToObject(cjson, "target", cJSON_CreateNumber(friend_id));
    cJSON_AddItemToObject(cjson, "message", cJSON_CreateString(msg));
    /* send cjson */
    send_cjson(server, cjson);
    cJSON_Delete(cjson);
    cjson = recv_cjson(server, server_buff, server_buff_remain);
    state s = cJSON_GetObjectItem(cjson, "type")->valueint;
}

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

state add_friend(const int id)
{
    /* create cjson of add friend request */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(ADD_FRIEND_REQUEST));
    cJSON_AddItemToObject(cjson, "id", cJSON_CreateNumber(my_info.my_profile.id));
    cJSON_AddItemToObject(cjson, "friend_id", cJSON_CreateNumber(id));
    /* send cjson */
    send_cjson(server, cjson);
    cJSON_Delete(cjson);
    cjson = recv_cjson(server, server_buff, &server_buff_remain);
    state s = cJSON_GetObjectItem(cjson, "type")->valueint;
    cJSON_Delete(cjson);
    return s;
}
state logout()
{
    /* create cjson of logout request */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(LOGOUT));
    cJSON_AddItemToObject(cjson, "id", cJSON_CreateNumber(my_info.my_profile.id));
    /* send cjson */
    send_cjson(server, cjson);
    cJSON_Delete(cjson);
    cjson = recv_cjson(server, server_buff, &server_buff_remain);
    state s = cJSON_GetObjectItem(cjson, "type")->valueint;
    if(s == SUCCESS) 
    {
        /* release resource */
        close(server);
    }
    cJSON_Delete(cjson);
    return s;
}

int main(int argc, char* argv[])
{
    /* connect to server */
    if((server = conn_to( inet_addr("127.0.0.1"), SERVER_PORT)) == -1)
    {
        printf("cannot connect to server, try again later.\n");
        return -1;
    }
    /* test regist, login and send message */
    //printf("regist result: %d\n", regist("pengyao", "123456"));
    printf("login result: %d\n", login(10000, "123456"));
    printf("add friend result: %d\n", add_friend(10001));
    //printf("send message to friend result: %d\n", send_msg_to_friend(10001, "hello my friend"));
    printf("logout result: %d\n", logout());
    //send_msg(server, "pengyao", "helloworld!");



    //int socket = conn_to(SERVER_IP, SERVER_PORT);
    /* test send file */
    //send_file(socket, "/home/onlyrobot/t/test.txt", \
    get_file_size("/home/onlyrobot/t/test.txt"), NULL);
    //close(server);
    //close(socket);
    return 0;
}