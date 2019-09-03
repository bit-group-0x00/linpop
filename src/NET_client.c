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

void handle_msg_recv(int client, cJSON* cjson);

void handle_add_fri_request(int client, cJSON* cjson);

void handle_join_gro_request(int client, cJSON* cjson);

message* parse_msg(cJSON* cjson);

friend* parse_fri(cJSON* cjson);

friend* seek_fri(int id);

void append_fri(friend* fri);

void append_msg_to_fri(friend* fri, message* msg);

void append_msg_to_gro(group* gro, message* msg);

void append_mem_to_gro(group* gro, member* mem);

group* parse_gro(cJSON* cjson);

void append_gro(group* gro)
{
    if(my_info.last_gro != NULL)
    {
        my_info.last_gro->next = gro;
        gro->last = my_info.last_gro;
        my_info.last_gro = gro;
    } else my_info.last_gro = my_info.first_gro = gro;
}

void append_mem_to_gro(group* gro, member* mem)
{
    if(gro == NULL) return;
    if(gro->last_mem != NULL)
    {
        gro->last_mem->next = mem;
        mem->last = gro->last_mem;
        gro->last_mem = mem;
    } else gro->last_mem = gro->first_mem = mem;
}

group* parse_gro(cJSON* cjson)
{
    group* gro = malloc(sizeof(group));
    gro->first_mem = gro->last_mem = NULL;
    gro->first_msg = gro->last_msg = NULL;
    gro->last = gro->next = NULL;
    gro->gro_pro.id = cJSON_GetObjectItem(cjson, "group_id")->valueint;
    gro->gro_pro.name = copy(cJSON_GetObjectItem(cjson, "name")->valuestring);
    gro->gro_pro.intro = copy(cJSON_GetObjectItem(cjson, "intro")->valuestring);
    gro->gro_pro.icon = copy(cJSON_GetObjectItem(cjson, "icon")->valuestring);
    int member_num = cJSON_GetObjectItem(cjson, "member_num")->valueint;

    cJSON* ids = cJSON_GetObjectItem(cjson, "member_ids");
    cJSON* nick_names = cJSON_GetObjectItem(cjson, "nick_names");
    cJSON* avatars = cJSON_GetObjectItem(cjson, "avatars");
    cJSON* ips = cJSON_GetObjectItem(cjson, "ips");
    cJSON* states = cJSON_GetObjectItem(cjson, "states");
    cJSON* signatures = cJSON_GetObjectItem(cjson, "signatures");
    for(int i = 0; i < member_num; ++i)
    {
        member* mem = malloc(sizeof(profile));
        mem->mem_pro.id = cJSON_GetArrayItem(ids, i)->valueint;
        friend* fri = seek_fri(mem->mem_pro.id);
        /* if user's profie already exist */
        if(fri != NULL) mem->mem_pro = fri->fri_pro;
        else
        {
            mem->mem_pro.nick_name = copy(cJSON_GetArrayItem(nick_names, i)->valuestring);
            mem->mem_pro.avatar = copy(cJSON_GetArrayItem(avatars, i)->valuestring);
            mem->mem_pro.ip = copy(cJSON_GetArrayItem(ips, i)->valuestring);
            mem->mem_pro.online = cJSON_GetArrayItem(states, i)->valueint;
            mem->mem_pro.signature = copy(cJSON_GetArrayItem(signatures, i)->valuestring);
        }
        append_mem_to_gro(gro, mem);
    }
    return gro;
}

void append_msg_to_gro(group* gro, message* msg)
{
    if(gro == NULL) return;
    if(gro->last_msg != NULL)
    {
        gro->last_msg->next = msg;
        msg->last = gro->last_msg;
        gro->last_msg = msg;
    } else gro->last_msg = gro->first_msg = msg;
}

void append_msg_to_fri(friend* fri, message* msg)
{
    if(fri == NULL) return;
    if(fri->last_msg != NULL)
    {
        fri->last_msg->next = msg;
        msg->last = fri->last_msg;
        fri->last_msg = msg;
    } else fri->last_msg = fri->first_msg = msg;
}

void append_fri(friend* fri)
{
    if(my_info.last_fri != NULL)
    {
        my_info.last_fri->next = fri;
        fri->last = my_info.last_fri;
        my_info.last_fri = fri;
    } else my_info.last_fri = my_info.first_fri = fri;
}

group* seek_gro(int id)
{
    group* gro = my_info.first_gro;
    while(gro != NULL)
    {
        if(gro->gro_pro.id == id) break;
        else gro = gro->next;
    }
    return gro; 
}

friend* seek_fri(int id)
{
    friend* fri = my_info.first_fri;
    while(fri != NULL)
    {
        if(fri->fri_pro.id == id) break;
        else fri = fri->next;
    }
    return fri;
}

message* parse_msg(cJSON* cjson)
{
    message* msg = malloc(sizeof(message));
    msg->last = msg->next = NULL;
    msg->sender = cJSON_GetObjectItem(cjson, "sender")->valueint;
    msg->checked = cJSON_GetObjectItem(cjson, "checked")->valueint;
    msg->date = copy(cJSON_GetObjectItem(cjson, "date")->valuestring);
    msg->content = copy(cJSON_GetObjectItem(cjson, "content"));
    return msg;
}

friend* parse_fri(cJSON* cjson)
{
    friend* fri = malloc(sizeof(friend));
    fri->last_msg = fri->first_msg = NULL;
    fri->last_msg = fri->first_msg = NULL;
    fri->fri_pro.id = cJSON_GetObjectItem(cjson, "id")->valueint;
    fri->fri_pro.online = cJSON_GetObjectItem(cjson, "online")->valueint;
    fri->fri_pro.ip = copy(cJSON_GetObjectItem(cjson, "ip")->valuestring);
    fri->fri_pro.nick_name = copy(cJSON_GetObjectItem(cjson, "nick_name")->valuestring);
    fri->fri_pro.signature = copy(cJSON_GetObjectItem(cjson, "signature")->valuestring);
    fri->fri_pro.avatar = copy(cJSON_GetObjectItem(cjson, "avatar")->valuestring);
    return fri;
}


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
        my_info.my_pro.nick_name = copy(cJSON_GetObjectItem(cjson, "nick_name")->valuestring);
        my_info.my_pro.avatar = copy(cJSON_GetObjectItem(cjson, "avatar")->valuestring);
        my_info.my_pro.online = cJSON_GetObjectItem(cjson, "state")->valueint;
        my_info.my_pro.ip = copy(cJSON_GetObjectItem(cjson, "ip")->valuestring);
        my_info.my_pro.signature = copy(cJSON_GetObjectItem(cjson, "signature")->valuestring);
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
    cJSON_AddItemToObject(request_friend_list, "id", cJSON_CreateNumber(/*my_info.my_pro.*/id));
    /* todo: here needs more varify information */
    send_cjson(server, request_friend_list);
    cJSON_Delete(request_friend_list);
    cJSON* friend_list_cjson = recv_cjson(server, server_buff, &server_buff_remain);
    //printf("%s\n", cJSON_Print(friend_list_cjson));
    state s = cJSON_GetObjectItem(friend_list_cjson, "type")->valueint;
    if(s == SUCCESS)
    {
        int friend_num = cJSON_GetObjectItem(friend_list_cjson, "friend_num")->valueint;
        my_info.first_fri = my_info.last_fri = NULL;
        //my_info.friends = malloc(my_info.friend_num * sizeof(friend));
        cJSON* ids = cJSON_GetObjectItem(friend_list_cjson, "ids");
        cJSON* nick_names = cJSON_GetObjectItem(friend_list_cjson, "nick_names");
        cJSON* avatars = cJSON_GetObjectItem(friend_list_cjson, "avatars");
        cJSON* ips = cJSON_GetObjectItem(friend_list_cjson, "ips");
        cJSON* states = cJSON_GetObjectItem(friend_list_cjson, "states");
        cJSON* signatures = cJSON_GetObjectItem(friend_list_cjson, "signatures");
        for(int i = 0; i < friend_num; ++i)
        {
            friend* fri = malloc(sizeof(friend));
            fri->fri_pro.id = cJSON_GetArrayItem(ids, i)->valueint;
            fri->fri_pro.nick_name = copy(cJSON_GetArrayItem(nick_names, i)->valuestring);
            fri->fri_pro.avatar = copy(cJSON_GetArrayItem(avatars, i)->valuestring);
            fri->fri_pro.ip = copy(cJSON_GetArrayItem(ips, i)->valuestring);
            fri->fri_pro.online = cJSON_GetArrayItem(states, i)->valueint;
            fri->fri_pro.signature = copy(cJSON_GetArrayItem(signatures, i)->valuestring);
            fri->first_msg = fri->last_msg = NULL;
            cJSON* request_msg_list = cJSON_CreateObject();
            cJSON_AddItemToObject(request_msg_list, "type", cJSON_CreateNumber(REQUEST_MESSAGE_LIST));
            cJSON_AddItemToObject(request_msg_list, "id", cJSON_CreateNumber(/*my_info.my_pro.*/id));
            cJSON_AddItemToObject(request_msg_list, "friend_id", cJSON_CreateNumber(fri->fri_pro.id));
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
                if(fri->last_msg == NULL) fri->last_msg = fri->first_msg = msg;
                else
                {
                    /* add new message to the end of linked list */
                    fri->last_msg->next = msg;
                    msg->last = fri->last_msg;
                    fri->last_msg = msg;
                }
            }
            cJSON_Delete(msg_list_cjson);
            if(my_info.last_fri == NULL) my_info.last_fri = my_info.first_fri = fri;
            else
            {
                my_info.last_fri->next = fri;
                fri->last = my_info.last_fri;
                my_info.last_fri = fri;
            }
        }
    }

    cJSON* request_group_list = cJSON_CreateObject();
    cJSON_AddItemToObject(request_group_list, "type", cJSON_CreateNumber(REQUEST_GROUP_LIST));
    cJSON_AddItemToObject(request_group_list, "id", cJSON_CreateNumber(id));
    /* todo: here needs more varify information */
    send_cjson(server, request_group_list);
    cJSON_Delete(request_group_list);
    cJSON* group_list_cjson = recv_cjson(server, server_buff, &server_buff_remain);
    s = cJSON_GetObjectItem(group_list_cjson, "type")->valueint;
    if(s == SUCCESS)
    {
        int group_num = cJSON_GetObjectItem(group_list_cjson, "group_num")->valueint;
        my_info.first_gro = my_info.last_gro = NULL;
        //my_info.groups = malloc(my_info.group_num * sizeof(group));
        cJSON* g_ids = cJSON_GetObjectItem(group_list_cjson, "group_ids");
        cJSON* names = cJSON_GetObjectItem(group_list_cjson, "names");
        cJSON* intros = cJSON_GetObjectItem(group_list_cjson, "intros");
        cJSON* icons = cJSON_GetObjectItem(group_list_cjson, "icons");
        for(int i = 0; i < group_num; ++i)
        {
            group* gro = malloc(sizeof(group));
            gro->gro_pro.id = cJSON_GetArrayItem(g_ids, i)->valueint;
            gro->gro_pro.name = copy(cJSON_GetArrayItem(names, i)->valuestring);
            gro->gro_pro.intro = copy(cJSON_GetArrayItem(intros, i)->valuestring);
            gro->gro_pro.icon = copy(cJSON_GetArrayItem(icons, i)->valuestring);
            gro->last_msg = gro->first_msg = NULL;
            cJSON* request_g_msg_list = cJSON_CreateObject();
            cJSON_AddItemToObject(request_g_msg_list, "type", cJSON_CreateNumber(REQUEST_GROUP_MESSAGE_LIST));
            cJSON_AddItemToObject(request_g_msg_list, "id", cJSON_CreateNumber(id));
            cJSON_AddItemToObject(request_g_msg_list, "group_id", cJSON_CreateNumber(gro->gro_pro.id));
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
                if(gro->last_msg == NULL) gro->last_msg = gro->first_msg = msg;
                else
                {
                    /* add new message to the end of linked list */
                    gro->last_msg->next = msg;
                    msg->last = gro->last_msg;
                    gro->last_msg = msg;
                }
            }
            cJSON_Delete(g_msg_list_cjson);

            cJSON* request_g_member_list_cjson = cJSON_CreateObject();
            cJSON_AddItemToObject(request_g_member_list_cjson, "type", cJSON_CreateNumber(REQUEST_GROUP_MEMBER_LIST));
            cJSON_AddItemToObject(request_g_member_list_cjson, "id", cJSON_CreateNumber(id));
            cJSON_AddItemToObject(request_g_member_list_cjson, "group_id", cJSON_CreateNumber(gro->gro_pro.id));
            send_cjson(server, request_g_member_list_cjson);
            cJSON_Delete(request_g_member_list_cjson);
            cJSON* g_member_list_cjson = recv_cjson(server, server_buff, &server_buff_remain);
            

            int member_num = cJSON_GetObjectItem(g_member_list_cjson, "member_num")->valueint;
            gro->first_mem = gro->last_mem = NULL;
            //my_info.groups[i].members = malloc(my_info.groups[i].member_num * sizeof(profile));
            cJSON* ids = cJSON_GetObjectItem(g_member_list_cjson, "member_ids");
            cJSON* nick_names = cJSON_GetObjectItem(g_member_list_cjson, "nick_names");
            cJSON* avatars = cJSON_GetObjectItem(g_member_list_cjson, "avatars");
            cJSON* ips = cJSON_GetObjectItem(g_member_list_cjson, "ips");
            states = cJSON_GetObjectItem(g_member_list_cjson, "states");
            //cJSON* states = cJSON_GetObjectItem(g_member_list_cjson, "states");
            cJSON* signatures = cJSON_GetObjectItem(g_member_list_cjson, "signatures");
            for(int j = 0; j < member_num; ++j)
            {
                member* mem = malloc(sizeof(profile));
                mem->mem_pro.id = cJSON_GetArrayItem(ids, j)->valueint;
                mem->mem_pro.nick_name = copy(cJSON_GetArrayItem(nick_names, j)->valuestring);
                mem->mem_pro.avatar = copy(cJSON_GetArrayItem(avatars, j)->valuestring);
                mem->mem_pro.ip = copy(cJSON_GetArrayItem(ips, j)->valuestring);
                mem->mem_pro.online = cJSON_GetArrayItem(states, j)->valueint;
                mem->mem_pro.signature = copy(cJSON_GetArrayItem(signatures, j)->valuestring);
                if(gro->last_mem == NULL) gro->last_mem = gro->first_mem = mem;
                else
                {
                    /* add member to linked list */
                    gro->last_mem->next = mem;
                    mem->last = gro->last_mem;
                    gro->last_mem = mem;
                }
            }
            cJSON_Delete(g_member_list_cjson);

            if(my_info.last_gro == NULL) my_info.last_gro = my_info.first_gro = gro;
            else
            {
                /* add new group to the end of linked list */
                my_info.last_gro->next = gro;
                gro->last = my_info.last_gro;
                my_info.last_gro = gro;
            }
        }
    }
    //cJSON_Delete(cjson);
    return s;
}

state create_group(char* name, char* intro, char* icon, int member_num, int *member_ids)
{
    /* create cjson of create group request */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(CREATE_GROUP_REQUEST));
    cJSON_AddItemToObject(cjson, "id", cJSON_CreateNumber(my_info.my_pro.id));
    cJSON_AddItemToObject(cjson, "name", cJSON_CreateString(name));
    cJSON_AddItemToObject(cjson, "intro", cJSON_CreateString(intro));
    cJSON_AddItemToObject(cjson, "icon", cJSON_CreateString(icon));
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
    cJSON_AddItemToObject(cjson, "id", cJSON_CreateNumber(my_info.my_pro.id));
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
    cJSON_AddItemToObject(cjson, "sender", cJSON_CreateNumber(my_info.my_pro.id));
    cJSON_AddItemToObject(cjson, "target", cJSON_CreateNumber(friend_id));
    cJSON_AddItemToObject(cjson, "content", cJSON_CreateString(msg));
    /* send cjson */
    send_cjson(server, cjson);
    cJSON_Delete(cjson);
    cjson = recv_cjson(server, server_buff, server_buff_remain);
    state s = cJSON_GetObjectItem(cjson, "type")->valueint;
}

void handle_msg_recv(int socket, cJSON* cjson)
{
    printf("before");
    message* msg = parse_msg(cjson);
    printf("here");
    /* friend message */
    if(msg->sender >= 9999)
    {
        friend* fri = seek_fri(msg->sender);
        append_msg_to_fri(fri, msg);
    } else /* group message */
    {
        group* gro = seek_gro(msg->sender);
        append_msg_to_gro(gro, msg);
    }
    response_state(socket, SUCCESS);
    my_info.update_ui(SEND_MESSAGE, msg);
}

void handle_add_fri_request(int socket, cJSON* cjson)
{
    friend* fri = parse_fri(cjson);
    append_fri(fri);
    response_state(socket, SUCCESS);
    my_info.update_ui(ADD_FRIEND_REQUEST, fri);
}

void handle_join_gro_request(int client, cJSON* cjson)
{
    /* todo */
    group* gro = parse_gro(cjson);
    append_gro(gro);
    my_info.update_ui(CREATE_GROUP_REQUEST, gro);
}

void handle_cjson(int socket, cJSON* cjson)
{
    printf("here");
    cJSON* type = cJSON_GetObjectItem(cjson, "type");
    void (*handle)(int, cJSON*);
    switch(type->valueint)
    {
        case SEND_MESSAGE: handle = handle_msg_recv; break;
        case ADD_FRIEND_REQUEST: handle = handle_add_fri_request; break;
        case CREATE_GROUP_REQUEST: handle = handle_join_gro_request; break;
        default:
        {
            printf("recieved unknown type message:\n");
            char* s = cJSON_Print(cjson);
            printf("%s\n", s);
            free(s);
        }
    }
    handle(socket, cjson);
}

state add_friend(const int id)
{
    /* create cjson of add friend request */
    cJSON* cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(cjson, "type", cJSON_CreateNumber(ADD_FRIEND_REQUEST));
    cJSON_AddItemToObject(cjson, "id", cJSON_CreateNumber(my_info.my_pro.id));
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
    cJSON_AddItemToObject(cjson, "id", cJSON_CreateNumber(my_info.my_pro.id));
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

void update_ui(state type, void* origin)
{
    printf("type is %d\n", type);
}

int main(int argc, char* argv[])
{
    pthread_t thread;
    struct args arg;
    arg.value = CLIENT_PORT, arg.handle = handle_cjson;
    pthread_create(&thread, NULL, monitor_port, (void*)&arg);
    //monitor_port((void*)&arg);


    /* connect to server */
    if((server = conn_to( inet_addr(DEFAULT_SERVER_IP), SERVER_PORT)) == -1)
    {
        printf("cannot connect to server, try again later.\n");
        return -1;
    }
    int id = 10000;
    my_info.update_ui = update_ui;
    /* test regist, login and send message */
    //printf("regist result: %d\n", regist("helloworld", "123456"));
    printf("login result: %d\n", login(id, "123456"));
    my_info.my_pro.id = id;
    int group_members[5] = { 10000, 10001, 10002, 10003, 10004 };
    printf("create group result: %d\n", create_group("linpop", "linpop group", "icon_path", 5, group_members));
    //printf("join group result: %d\n", join_group(1));
    //printf("add friend result: %d\n", add_friend(10001));
    //printf("send message to friend result: %d\n", send_msg_to_friend(10001, "hello my friend"));
    printf("logout result: %d\n", logout());
    //send_msg(server, "pengyao", "helloworld!");



    //int socket = conn_to(SERVER_IP, SERVER_PORT);
    /* test send file */
    //send_file(socket, "/home/onlyrobot/t/test.txt", \
    get_file_size("/home/onlyrobot/t/test.txt"), NULL);
    //close(server);
    //close(socket);
    while(1);
    return 0;
}