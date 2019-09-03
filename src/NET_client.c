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
state request_my_info(int id);

/* copy a char* from s */
char* copy(char* s);

char* copy(char* s)
{
    char* res = malloc(sizeof(char) * (strlen(s) + 1));
    strcpy(res, s);
    return res;
}

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
    if(s == SUCCESS)
    {
        int id = cJSON_GetObjectItem(cjson, "id")->valueint;
        cJSON_Delete(cjson);
        return id;
    } else
    {
        cJSON_Delete(cjson);
        return s;
    }
}

state login(int id, const char* passwd)
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
        my_info.my_profile.nick_name = copy(cJSON_GetObjectItem(cjson, "nick_name")->valuestring);
        my_info.my_profile.avatar = copy(cJSON_GetObjectItem(cjson, "avatar")->valuestring);
        my_info.my_profile.online = cJSON_GetObjectItem(cjson, "state")->valueint;
        my_info.my_profile.ip = copy(cJSON_GetObjectItem(cjson, "ip")->valuestring);
        my_info.my_profile.signature = copy(cJSON_GetObjectItem(cjson, "signature")->valuestring);
        s = request_my_info(id);
    }
    cJSON_Delete(cjson);
    return s;
}

state request_my_info(int id)
{
    /* create cjson of my info request */
    cJSON* request_friend_list = cJSON_CreateObject();
    cJSON_AddItemToObject(request_friend_list, "type", cJSON_CreateNumber(REQUEST_FRIEND_LIST));
    cJSON_AddItemToObject(request_friend_list, "id", cJSON_CreateNumber(/*my_info.my_profile.*/id));
    /* todo: here needs more varify information */
    send_cjson(server, request_friend_list);
    cJSON_Delete(request_friend_list);
    cJSON* friend_list_cjson = recv_cjson(server, server_buff, &server_buff_remain);
    //printf("%s\n", cJSON_Print(friend_list_cjson));
    state s = cJSON_GetObjectItem(friend_list_cjson, "type")->valueint;
    if(s == SUCCESS)
    {
        my_info.friend_num = cJSON_GetObjectItem(friend_list_cjson, "friend_num")->valueint;
        my_info.friends = malloc(my_info.friend_num * sizeof(friend));
        cJSON* ids = cJSON_GetObjectItem(friend_list_cjson, "ids");
        cJSON* nick_names = cJSON_GetObjectItem(friend_list_cjson, "nick_names");
        cJSON* avatars = cJSON_GetObjectItem(friend_list_cjson, "avatars");
        cJSON* ips = cJSON_GetObjectItem(friend_list_cjson, "ips");
        cJSON* states = cJSON_GetObjectItem(friend_list_cjson, "states");
        cJSON* signatures = cJSON_GetObjectItem(friend_list_cjson, "signatures");
        for(int i = 0; i < my_info.friend_num; ++i)
        {
            my_info.friends[i].friend_profile.id = cJSON_GetArrayItem(ids, i)->valueint;
            my_info.friends[i].friend_profile.nick_name = copy(cJSON_GetArrayItem(nick_names, i)->valuestring);
            my_info.friends[i].friend_profile.avatar = copy(cJSON_GetArrayItem(avatars, i)->valuestring);
            my_info.friends[i].friend_profile.ip = copy(cJSON_GetArrayItem(ips, i)->valuestring);
            my_info.friends[i].friend_profile.online = cJSON_GetArrayItem(states, i)->valueint;
            my_info.friends[i].friend_profile.signature = copy(cJSON_GetArrayItem(signatures, i)->valuestring);
            my_info.friends[i].first_msg = my_info.friends[i].last_msg = NULL;
            cJSON* request_msg_list = cJSON_CreateObject();
            cJSON_AddItemToObject(request_msg_list, "type", cJSON_CreateNumber(REQUEST_MESSAGE_LIST));
            cJSON_AddItemToObject(request_msg_list, "id", cJSON_CreateNumber(/*my_info.my_profile.*/id));
            cJSON_AddItemToObject(request_msg_list, "friend_id", cJSON_CreateNumber(my_info.friends[i].friend_profile.id));
            send_cjson(server, request_msg_list);
            cJSON_Delete(request_msg_list);
            cJSON* msg_list_cjson = recv_cjson(server, server_buff, &server_buff_remain);
            //printf("%s", cJSON_Print(msg_list_cjson));
            int msg_num = cJSON_GetObjectItem(msg_list_cjson, "message_num")->valueint;
            cJSON* contents = cJSON_GetObjectItem(msg_list_cjson, "contents");
            cJSON* senders = cJSON_GetObjectItem(msg_list_cjson, "senders");
            cJSON* dates = cJSON_GetObjectItem(msg_list_cjson, "dates");
            cJSON* states = cJSON_GetObjectItem(msg_list_cjson, "states");
            for(int j = 0; j < msg_num; ++j)
            {
                message* msg = malloc(sizeof(message));
                msg->last = msg->next = NULL;
                msg->sender = cJSON_GetArrayItem(senders, j)->valueint;
                msg->checked = cJSON_GetArrayItem(states, j)->valueint;
                msg->content = copy(cJSON_GetArrayItem(contents, j)->valuestring);
                msg->date = copy(cJSON_GetArrayItem(dates, j)->valuestring);
                if(my_info.friends[i].last_msg == NULL)
                {
                    my_info.friends[i].last_msg = msg;
                    my_info.friends[i].first_msg = msg;
                }
                else
                {
                    /* add new message to the end of list */
                    my_info.friends[i].last_msg->next = msg;
                    msg->last = my_info.friends[j].last_msg;
                    my_info.friends[i].last_msg = msg;
                }
            }
            cJSON_Delete(msg_list_cjson);
        }
    }

    cJSON* request_group_list = cJSON_CreateObject();
    cJSON_AddItemToObject(request_group_list, "type", cJSON_CreateNumber(REQUEST_GROUP_LIST));
    cJSON_AddItemToObject(request_group_list, "id", cJSON_CreateNumber(my_info.my_profile.id));
    /* todo: here needs more varify information */
    send_cjson(server, request_group_list);
    cJSON_Delete(request_group_list);
    cJSON* group_list_cjson = recv_cjson(server, server_buff, &server_buff_remain);
    s = cJSON_GetObjectItem(group_list_cjson, "type")->valueint;
    if(s == SUCCESS)
    {
        my_info.group_num = cJSON_GetObjectItem(group_list_cjson, "group_num")->valueint;
        my_info.groups = malloc(my_info.group_num * sizeof(group));
        cJSON* g_ids = cJSON_GetObjectItem(group_list_cjson, "ids");
        cJSON* names = cJSON_GetObjectItem(group_list_cjson, "names");
        cJSON* intros = cJSON_GetObjectItem(group_list_cjson, "intros");
        cJSON* icons = cJSON_GetObjectItem(group_list_cjson, "icons");
        for(int i = 0; i < my_info.group_num; ++i)
        {
            my_info.groups[i].g_profile.id = cJSON_GetArrayItem(g_ids, i)->valueint;
            my_info.groups[i].g_profile.name = copy(cJSON_GetArrayItem(names, i)->valuestring);
            my_info.groups[i].g_profile.intro = copy(cJSON_GetArrayItem(intros, i)->valuestring);
            my_info.groups[i].g_profile.icon = copy(cJSON_GetArrayItem(icons, i)->valuestring);
            my_info.groups[i].first_msg = my_info.groups[i].last_msg = NULL;


            cJSON* request_g_msg_list = cJSON_CreateObject();
            cJSON_AddItemToObject(request_g_msg_list, "type", cJSON_CreateNumber(REQUEST_GROUP_MESSAGE_LIST));
            cJSON_AddItemToObject(request_g_msg_list, "id", cJSON_CreateNumber(my_info.my_profile.id));
            cJSON_AddItemToObject(request_g_msg_list, "group_id", cJSON_CreateNumber(my_info.groups[i].g_profile.id));
            send_cjson(server, request_g_msg_list);
            cJSON_Delete(request_g_msg_list);

            cJSON* g_msg_list_cjson = recv_cjson(server, server_buff, &server_buff_remain);
            //printf("%s", cJSON_Print(g_msg_list_cjson));
            int msg_num = cJSON_GetObjectItem(g_msg_list_cjson, "message_num")->valueint;
            cJSON* contents = cJSON_GetObjectItem(g_msg_list_cjson, "contents");
            cJSON* senders = cJSON_GetObjectItem(g_msg_list_cjson, "senders");
            cJSON* dates = cJSON_GetObjectItem(g_msg_list_cjson, "dates");
            cJSON* states = cJSON_GetObjectItem(g_msg_list_cjson, "states");
            for(int j = 0; j < msg_num; ++j)
            {
                message* msg = malloc(sizeof(message));
                msg->last = msg->next = NULL;
                msg->sender = cJSON_GetArrayItem(senders, j)->valueint;
                msg->checked = cJSON_GetArrayItem(states, j)->valueint;
                msg->content = copy(cJSON_GetArrayItem(contents, j)->valuestring);
                msg->date = copy(cJSON_GetArrayItem(dates, j)->valuestring);
                if(my_info.groups[i].last_msg == NULL)
                {
                    my_info.groups[i].last_msg = msg;
                    my_info.groups[i].first_msg = msg;
                }
                else
                {
                    /* add new message to the end of list */
                    my_info.groups[i].last_msg->next = msg;
                    msg->last = my_info.groups[j].last_msg;
                    my_info.groups[i].last_msg = msg;
                }
            }
            cJSON_Delete(g_msg_list_cjson);


            cJSON* request_g_member_list_cjson = cJSON_CreateObject();
            cJSON_AddItemToObject(request_g_member_list_cjson, "type", cJSON_CreateNumber(REQUEST_GROUP_MEMBER_LIST));
            cJSON_AddItemToObject(request_g_member_list_cjson, "id", cJSON_CreateNumber(my_info.my_profile.id));
            cJSON_AddItemToObject(request_g_member_list_cjson, "group_id", cJSON_CreateNumber(my_info.groups[i].g_profile.id));
            send_cjson(server, request_g_member_list_cjson);
            cJSON_Delete(request_g_member_list_cjson);
            cJSON* g_member_list_cjson = recv_cjson(server, server_buff, &server_buff_remain);
            my_info.groups[i].member_num = cJSON_GetObjectItem(g_member_list_cjson, "member_num")->valueint;
            my_info.groups[i].members = malloc(my_info.groups[i].member_num * sizeof(profile));
            cJSON* ids = cJSON_GetObjectItem(g_member_list_cjson, "ids");
            cJSON* nick_names = cJSON_GetObjectItem(g_member_list_cjson, "nick_names");
            cJSON* avatars = cJSON_GetObjectItem(g_member_list_cjson, "avatars");
            cJSON* ips = cJSON_GetObjectItem(g_member_list_cjson, "ips");
            states = cJSON_GetObjectItem(g_member_list_cjson, "states");
            //cJSON* states = cJSON_GetObjectItem(g_member_list_cjson, "states");
            cJSON* signatures = cJSON_GetObjectItem(g_member_list_cjson, "signatures");
            for(int j = 0; j < my_info.groups[j].member_num; ++j)
            {
                my_info.groups[i].members[j].id = cJSON_GetArrayItem(ids, i)->valueint;
                my_info.groups[i].members[j].nick_name = copy(cJSON_GetArrayItem(nick_names, i)->valuestring);
                my_info.groups[i].members[j].avatar = copy(cJSON_GetArrayItem(avatars, i)->valuestring);
                my_info.groups[i].members[j].ip = copy(cJSON_GetArrayItem(ips, i)->valuestring);
                my_info.groups[i].members[j].online = cJSON_GetArrayItem(states, i)->valueint;
                my_info.groups[i].members[j].signature = copy(cJSON_GetArrayItem(signatures, i)->valuestring);
            }
            cJSON_Delete(g_member_list_cjson);
        }
    }
    //cJSON_Delete(cjson);
    return s;
}

state create_group(char* name, char* intro, char* avatar, int member_num, int *member_ids)
{
    /* create cjson of create group request */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(CREATE_GROUP_REQUEST));
    cJSON_AddItemToObject(cjson, "id", cJSON_CreateNumber(my_info.my_profile.id));
    cJSON_AddItemToObject(cjson, "name", cJSON_CreateString(name));
    cJSON_AddItemToObject(cjson, "intro", cJSON_CreateString(intro));
    cJSON_AddItemToObject(cjson, "icon", cJSON_CreateString(avatar));
    cJSON_AddItemToObject(cjson, "member_num", cJSON_CreateNumber(member_num));
    cJSON_AddItemToObject(cjson, "member_ids", cJSON_CreateIntArray(member_ids, member_num));
    /* send cjson */
    send_cjson(server, cjson);
    cJSON_Delete(cjson);
    cjson = recv_cjson(server, server_buff, &server_buff_remain);
    state s = cJSON_GetObjectItem(cjson, "type")->valueint;
    if(s == SUCCESS)
    {
        int id = cJSON_GetObjectItem(cjson, "group_id")->valueint;
        cJSON_Delete(cjson);
        return id;
    } else
    {
        cJSON_Delete(cjson);
        return s;
    }
}

state join_group(int group_id)
{
    /* create cjson of join group request */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(JOIN_GROUP_REQUEST));
    cJSON_AddItemToObject(cjson, "id", cJSON_CreateNumber(my_info.my_profile.id));
    cJSON_AddItemToObject(cjson, "group_id", cJSON_CreateNumber(group_id));
    /* send cjson */
    send_cjson(server, cjson);
    cJSON_Delete(cjson);
    cjson = recv_cjson(server, server_buff, &server_buff_remain);
    state s = cJSON_GetObjectItem(cjson, "type")->valueint;
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
    if((server = conn_to( inet_addr(DEFAULT_SERVER_IP), SERVER_PORT)) == -1)
    {
        printf("cannot connect to server, try again later.\n");
        return -1;
    }
    int id = 10000;
    /* test regist, login and send message */
    //printf("regist result: %d\n", regist("helloworld", "123456"));
    printf("login result: %d\n", login(id, "123456"));
    my_info.my_profile.id = id;
    int group_members[5] = { 10000, 10001, 10002, 10003, 10004 };
    //printf("create group result: %d\n", create_group("linpop", "linpop group", "icon_path", 5, group_members));
    printf("join group result: %d\n", join_group(1));
    //printf("add friend result: %d\n", add_friend(10000));
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