/*
  File Name: NET_server.c
  Model Name: NET
  Create Date: 2019/8/29
  Author: PengYao
  Abstract Description: server side program
*/

#include "../include/NET_server.h"

MYSQL* conn = NULL;

/* 
  服务端的handle函数，在接受到注册请求后会自动调用该函数
  函数的参数三请求方的socket和接受到的cjson
*/
void handle_regist_request(int client, cJSON* cjson);

/*
  同上，在接受到登陆请求后会自动调用该函数
*/
void handle_login_request(int client, cJSON* cjson);

/*
  同上，在接受到登出请求后会自动调用该函数
*/
void handle_logout_request(int client, cJSON* cjson);

/* 
  同上，在接受到发送消息请求后自动调用该函数，其中发送消息
  可以分为两种：用户向其他用户发送消息、用户向群组发送消息
*/
void handle_msg_send(int client, cJSON* cjson);

/* 
  同上，在接受到发送文件请求后自动调用该函数，其中发送文件也可以分为两种
  一种是客户端向服务端发送文件，另一种是客户端向其他客户端发送文件
*/
void handle_file_send(int client, cJSON* cjson);

/*
  同上，在接受到请求好友列表的时候自动调用该函数。
*/
void handle_friend_list_request(int client, cJSON* cjson);

/*
  同上，在接受到请求消息列表时自动调用该函数
*/
void handle_msg_list_request(int client, cJSON* cjson);

/* 
同上，在接受到请求群组列表的时候会自动调用该函数
*/
void handle_group_list_request(int client, cJSON* cjson);

/*
  同上，在接受到添加好友请求时自动调用该函数
*/
void handle_add_friend_request(int client, cJSON* cjson);

void handle_group_message_list_request(int client, cJSON* cjson);

void handle_group_member_list_request(int client, cJSON* cjson);

void handle_create_group_request(int client, cJSON* cjson);

void handle_join_group_request(int client, cJSON* cjson);

void handle_regist_request(int client, cJSON* cjson)
{
    int id;
    User user;
    user.userNickName = cJSON_GetObjectItem(cjson, "nick_name")->valuestring;
    user.userPassword = cJSON_GetObjectItem(cjson, "passwd")->valuestring;
    user.userSignature = cJSON_GetObjectItem(cjson, "signature")->valuestring;
    user.userAvatar = cJSON_GetObjectItem(cjson, "avatar")->valuestring;
    user.userSignature = "";
    //user.userIp = -1;
    if((id = insertUser(&user, conn)) != -1)
    {
        /* 创建response的cjson */
        cJSON* response = cJSON_CreateObject();
        cJSON_AddItemToObject(response, "type", cJSON_CreateNumber(SUCCESS));
        cJSON_AddItemToObject(response, "id", cJSON_CreateNumber(id));
        send_cjson(client, response);
        cJSON_Delete(response);
    } else response_state(client, FAILURE);
}

void handle_create_group_request(int client, cJSON* cjson)
{
    int group_id;
    Group group;
    group.groupName = cJSON_GetObjectItem(cjson, "name")->valuestring;
    group.groupIntro = cJSON_GetObjectItem(cjson, "intro")->valuestring;
    group.groupIcon = cJSON_GetObjectItem(cjson, "icon")->valuestring;
    int member_num = cJSON_GetObjectItem(cjson, "member_num")->valueint;
    cJSON* member_ids = cJSON_GetObjectItem(cjson, "member_ids");
    cJSON* nick_names = cJSON_CreateArray();
    cJSON* avatars = cJSON_CreateArray();
    cJSON* ips = cJSON_CreateArray();
    cJSON* states = cJSON_CreateArray();
    cJSON* signatures = cJSON_CreateArray();
    //group.groupIp = -1;
    if((group_id = insertGroup(&group, conn)) != -1)
    {
        for(int i = 0; i < member_num; ++i)
        {
            int id = cJSON_GetArrayItem(member_ids, i)->valueint;
            insertGroupUser(group_id, id, conn);
            User* user = getUserInfoById(id, conn);
            cJSON_AddItemToArray(nick_names, cJSON_CreateString(user->userNickName));
            cJSON_AddItemToArray(avatars, cJSON_CreateString(user->userAvatar));
            cJSON_AddItemToArray(ips, cJSON_CreateString(get_aip_2(user->userIp)));
            cJSON_AddItemToArray(states, cJSON_CreateNumber(user->userStatus));
            cJSON_AddItemToArray(signatures, cJSON_CreateString(user->userSignature));
            freeUser(user);
            /* todo insert error handle */
        }
        cJSON_AddItemToObject(cjson, "group_id", cJSON_CreateNumber(group_id));
        cJSON_AddItemToObject(cjson, "nick_names", nick_names);
        cJSON_AddItemToObject(cjson, "avatars", avatars);
        cJSON_AddItemToObject(cjson, "ips", ips);
        cJSON_AddItemToObject(cjson, "states", states);
        cJSON_AddItemToObject(cjson, "signatures", signatures);
        for(int i = 0; i < member_num; ++i)
        {
            int id = cJSON_GetArrayItem(member_ids, i)->valueint;
            User* user = getUserInfoById(id, conn);
            if(user->userStatus == ONLINE)
            {
                int client_2 = conn_to(user->userIp, CLIENT_PORT);
                send_cjson(client_2, cjson);
                cJSON* cj_2 = recv_cjson(client_2, NULL, NULL);
                if(cj_2 != NULL) cJSON_Delete(cj_2);
                close(client_2);
            }
            freeUser(user);
        }
        /* 创建response的cjson */
        cJSON* response = cJSON_CreateObject();
        cJSON_AddItemToObject(response, "type", cJSON_CreateNumber(SUCCESS));
        cJSON_AddItemToObject(response, "group_id", cJSON_CreateNumber(group_id));
        send_cjson(client, response);
        cJSON_Delete(response);
    } else response_state(client, FAILURE);
}

void handle_join_group_request(int client, cJSON* cjson)
{
    int id = cJSON_GetObjectItem(cjson, "id")->valueint;
    int group_id = cJSON_GetObjectItem(cjson, "group_id")->valueint;
    if(insertGroupUser(group_id, id, conn) == 1) response_state(client, SUCCESS);
    else response_state(client, FAILURE);
}

void handle_login_request(int client, cJSON* cjson)
{
    int id = cJSON_GetObjectItem(cjson, "id")->valueint;
    char* passwd = cJSON_GetObjectItem(cjson, "passwd")->valuestring;
    if(isUserExist(id, conn) == 1 && checkUserPassword(id, passwd, conn) == 1 && \
    updateUserStatus(id, 1, conn) == 1 && updateUserIp(id, get_ip(client), conn) == 1)
    {
        cJSON* response = cJSON_CreateObject();
        User* user = getUserInfoById(id, conn);
        cJSON_AddItemToObject(response, "type", cJSON_CreateNumber(SUCCESS));
        //cJSON_AddItemToObject(response, "id", cJSON_CreateNumber(id));
        cJSON_AddItemToObject(response, "nick_name", cJSON_CreateString(user->userNickName));
        cJSON_AddItemToObject(response, "avatar", cJSON_CreateString(user->userAvatar));
        cJSON_AddItemToObject(response, "signature", cJSON_CreateString(user->userSignature));
        cJSON_AddItemToObject(response, "state", cJSON_CreateNumber(ONLINE));
        cJSON_AddItemToObject(response, "ip", cJSON_CreateString(get_aip(client)));
        send_cjson(client, response);
        freeUser(user);
        cJSON_Delete(response);
    } else response_state(client, FAILURE);
}

void handle_logout_request(int client, cJSON* cjson)
{
    int id = cJSON_GetObjectItem(cjson, "id")->valueint;
    state type = FAILURE;
    if(isUserExist(id, conn) == 1 && updateUserStatus(id, 0, conn) == 1) type = SUCCESS;
    response_state(client, type);
}

void handle_friend_list_request(int client, cJSON* cjson)
{
    int id = cJSON_GetObjectItem(cjson, "id")->valueint;
    FriendList friend_list = getFriList(id, conn);
    cJSON* friend_list_cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(friend_list_cjson, "type", cJSON_CreateNumber(SUCCESS));
    cJSON_AddItemToObject(friend_list_cjson, "friend_num", cJSON_CreateNumber(friend_list.friendsNum));
    cJSON_AddItemToObject(friend_list_cjson, "ids", cJSON_CreateIntArray(friend_list.friId, friend_list.friendsNum));
    cJSON* nick_names = cJSON_CreateArray(), *avatars = cJSON_CreateArray();
    cJSON* states = cJSON_CreateArray(), *ips = cJSON_CreateArray(), *signatures = cJSON_CreateArray();
    for(int i = 0; i < friend_list.friendsNum; ++i)
    {
        User* friend = getUserInfoById(friend_list.friId[i], conn);
        cJSON_AddItemToArray(nick_names, cJSON_CreateString(friend->userNickName));
        cJSON_AddItemToArray(avatars, cJSON_CreateString(friend->userAvatar));
        cJSON_AddItemToArray(states, cJSON_CreateNumber(friend->userStatus));
        char* aip = get_aip_2(friend->userIp);
        cJSON_AddItemToArray(ips, cJSON_CreateString(aip));
        //free(aip);
        cJSON_AddItemToArray(signatures, cJSON_CreateString(friend->userSignature));
        freeUser(friend);
    }
    freeFriList(friend_list);
    cJSON_AddItemToObject(friend_list_cjson, "nick_names", nick_names);
    cJSON_AddItemToObject(friend_list_cjson, "avatars", avatars);
    cJSON_AddItemToObject(friend_list_cjson, "states", states);
    cJSON_AddItemToObject(friend_list_cjson, "ips", ips);
    cJSON_AddItemToObject(friend_list_cjson, "signatures", signatures);
    send_cjson(client, friend_list_cjson);
    //printf("%s", cJSON_Print(friend_list_cjson));
    cJSON_Delete(friend_list_cjson);
}

void handle_msg_list_request(int client, cJSON* cjson)
{
    int id = cJSON_GetObjectItem(cjson, "id")->valueint;
    int friend_id = cJSON_GetObjectItem(cjson, "friend_id")->valueint;
    MessageList msg_list = getMsgList(id, friend_id, conn);
    cJSON* msg_list_cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(msg_list_cjson, "type", cJSON_CreateNumber(SUCCESS));
    cJSON_AddItemToObject(msg_list_cjson, "message_num", cJSON_CreateNumber(msg_list.msgNum));
    cJSON* senders = cJSON_CreateArray(), *contents = cJSON_CreateArray();
    cJSON* dates = cJSON_CreateArray(), *states = cJSON_CreateArray();
    for(int i = 0; i < msg_list.msgNum; ++i)
    {
        cJSON_AddItemToArray(senders, cJSON_CreateNumber(msg_list.msgs[i].msgFromId));
        cJSON_AddItemToArray(contents, cJSON_CreateString(msg_list.msgs[i].msgContent));
        cJSON_AddItemToArray(dates, cJSON_CreateString(msg_list.msgs[i].msgDateTime));
        cJSON_AddItemToArray(states, cJSON_CreateNumber(msg_list.msgs[i].msgStatus));
    }
    freeMsgList(msg_list);
    cJSON_AddItemToObject(msg_list_cjson, "senders", senders);
    cJSON_AddItemToObject(msg_list_cjson, "contents", contents);
    cJSON_AddItemToObject(msg_list_cjson, "dates", dates);
    cJSON_AddItemToObject(msg_list_cjson, "states", states);
    send_cjson(client, msg_list_cjson);
    cJSON_Delete(msg_list_cjson);
}

void handle_group_list_request(int client, cJSON* cjson)
{
    int id = cJSON_GetObjectItem(cjson, "id")->valueint;
    GroupList group_list = getGroupListByUserId(id, conn);
    cJSON* group_list_cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(group_list_cjson, "type", cJSON_CreateNumber(SUCCESS));
    cJSON_AddItemToObject(group_list_cjson, "group_num", cJSON_CreateNumber(group_list.groupNum));
    cJSON_AddItemToObject(group_list_cjson, "group_ids", cJSON_CreateIntArray(group_list.groupIds, group_list.groupNum));
    cJSON* names = cJSON_CreateArray(), *intros = cJSON_CreateArray();
    cJSON* icons = cJSON_CreateArray();
    for(int i = 0; i < group_list.groupNum; ++i)
    {
        Group* group = getGroupInfoByGroupId(group_list.groupIds[i], conn);
        cJSON_AddItemToArray(names, cJSON_CreateString(group->groupName));
        cJSON_AddItemToArray(intros, cJSON_CreateString(group->groupIntro));
        cJSON_AddItemToArray(icons, cJSON_CreateString(group->groupIcon));
        freeGroup(group);
    }
    freeGroupList(group_list);
    cJSON_AddItemToObject(group_list_cjson, "names", names);
    cJSON_AddItemToObject(group_list_cjson, "intros", intros);
    cJSON_AddItemToObject(group_list_cjson, "icons", icons);
    send_cjson(client, group_list_cjson);
    //printf("%s", cJSON_Print(group_list_cjson));
    cJSON_Delete(group_list_cjson);
}

void handle_group_message_list_request(int client, cJSON* cjson)
{
    int id = cJSON_GetObjectItem(cjson, "id")->valueint;
    int group_id = cJSON_GetObjectItem(cjson, "group_id")->valueint;
    GroupMessageList group_msg_list = getGmMsgList(group_id, conn);
    cJSON* group_msg_list_cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(group_msg_list_cjson, "type", cJSON_CreateNumber(SUCCESS));
    cJSON_AddItemToObject(group_msg_list_cjson, "message_num", cJSON_CreateNumber(group_msg_list.gmNum));
    cJSON* senders = cJSON_CreateArray(), *contents = cJSON_CreateArray();
    cJSON* dates = cJSON_CreateArray(), *states = cJSON_CreateArray();
    for(int i = 0; i < group_msg_list.gmNum; ++i)
    {
        cJSON_AddItemToArray(senders, cJSON_CreateNumber(group_msg_list.gmMsgs[i].gmFromId));
        cJSON_AddItemToArray(contents, cJSON_CreateString(group_msg_list.gmMsgs[i].gmContent));
        cJSON_AddItemToArray(dates, cJSON_CreateString(group_msg_list.gmMsgs[i].gmDateTime));
        cJSON_AddItemToArray(states, cJSON_CreateNumber(/*group_msg_list.gmMsgs[i].gmStatus*/CHECKED));
    }
    freeGroupMessageList(group_msg_list);
    cJSON_AddItemToObject(group_msg_list_cjson, "senders", senders);
    cJSON_AddItemToObject(group_msg_list_cjson, "contents", contents);
    cJSON_AddItemToObject(group_msg_list_cjson, "dates", dates);
    cJSON_AddItemToObject(group_msg_list_cjson, "states", states);
    send_cjson(client, group_msg_list_cjson);
    cJSON_Delete(group_msg_list_cjson);
}

void handle_group_member_list_request(int client, cJSON* cjson)
{
    int id = cJSON_GetObjectItem(cjson, "id")->valueint;
    int group_id = cJSON_GetObjectItem(cjson, "group_id")->valueint;
    GroupUserList group_member_list = getUserListByGroupId(group_id, conn);
    cJSON* group_member_list_cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(group_member_list_cjson, "type", cJSON_CreateNumber(SUCCESS));
    cJSON_AddItemToObject(group_member_list_cjson, "member_num", cJSON_CreateNumber(group_member_list.userNum));
    cJSON_AddItemToObject(group_member_list_cjson, "member_ids", cJSON_CreateIntArray(group_member_list.userIds, group_member_list.userNum));
    cJSON* nick_names = cJSON_CreateArray(), *avatars = cJSON_CreateArray();
    cJSON* states = cJSON_CreateArray(), *ips = cJSON_CreateArray(), *signatures = cJSON_CreateArray();
    for(int i = 0; i < group_member_list.userNum; ++i)
    {
        User* member = getUserInfoById(group_member_list.userIds[i], conn);
        cJSON_AddItemToArray(nick_names, cJSON_CreateString(member->userNickName));
        cJSON_AddItemToArray(avatars, cJSON_CreateString(member->userAvatar));
        cJSON_AddItemToArray(states, cJSON_CreateNumber(member->userStatus));
        char* aip = get_aip_2(member->userIp);
        cJSON_AddItemToArray(ips, cJSON_CreateString(aip));
        //free(aip);
        cJSON_AddItemToArray(signatures, cJSON_CreateString(member->userSignature));
        freeUser(member);
    }
    freeGroupUserList(group_member_list);
    cJSON_AddItemToObject(group_member_list_cjson, "nick_names", nick_names);
    cJSON_AddItemToObject(group_member_list_cjson, "avatars", avatars);
    cJSON_AddItemToObject(group_member_list_cjson, "states", states);
    cJSON_AddItemToObject(group_member_list_cjson, "ips", ips);
    cJSON_AddItemToObject(group_member_list_cjson, "signatures", signatures);
    send_cjson(client, group_member_list_cjson);
    //printf("%s", cJSON_Print(group_msg_list_cjson));
    cJSON_Delete(group_member_list_cjson);
}

void handle_add_friend_request(int client, cJSON* cjson)
{
    int id = cJSON_GetObjectItem(cjson, "id")->valueint;
    int friend_id = cJSON_GetObjectItem(cjson, "friend_id")->valueint;
    if(isFriends(id, friend_id, conn) == 1 || insertFriends(id, friend_id, conn) == 1)
    {
        User* fri = getUserInfoById(friend_id, conn);
        cJSON* response_cjson = cJSON_CreateObject();
        cJSON_AddItemToObject(response_cjson, "type", cJSON_CreateNumber(SUCCESS));
        cJSON_AddItemToObject(response_cjson, "id", cJSON_CreateNumber(fri->userId));
        cJSON_AddItemToObject(response_cjson, "online", cJSON_CreateNumber(fri->userStatus));
        cJSON_AddItemToObject(response_cjson, "ip", cJSON_CreateString(get_aip_2(fri->userId)));
        cJSON_AddItemToObject(response_cjson, "nick_name", cJSON_CreateString(fri->userNickName));
        cJSON_AddItemToObject(response_cjson, "signature", cJSON_CreateString(fri->userSignature));
        cJSON_AddItemToObject(response_cjson, "avatar", cJSON_CreateString(fri->userAvatar));
        send_cjson(client, response_cjson);
        cJSON_Delete(response_cjson);
        if(fri->userStatus == ONLINE)
        {
            User* user = getUserInfoById(id, conn);
            cJSON* cjson_2 = cJSON_CreateObject();
            cJSON_AddItemToObject(cjson_2, "type", cJSON_CreateNumber(ADD_FRIEND_REQUEST));
            cJSON_AddItemToObject(cjson_2, "id", cJSON_CreateNumber(user->userId));
            cJSON_AddItemToObject(cjson_2, "online", cJSON_CreateNumber(user->userStatus));
            char* aip = get_aip_2(user->userIp);
            cJSON_AddItemToObject(cjson_2, "ip", cJSON_CreateString(aip));
            //free(aip);
            cJSON_AddItemToObject(cjson_2, "nick_name", cJSON_CreateString(user->userNickName));
            cJSON_AddItemToObject(cjson_2, "signature", cJSON_CreateString(user->userSignature));
            cJSON_AddItemToObject(cjson_2, "avatar", cJSON_CreateString(user->userAvatar));
            freeUser(user);
            int client_2 = conn_to(fri->userIp, CLIENT_PORT);
            if(client_2 != -1)
            {
                send_cjson(client_2, cjson_2);
                cJSON_Delete(cjson_2);
                cJSON* cj_2 = recv_cjson(client_2, NULL, NULL);
                close(client_2);
                cJSON_Delete(cj_2);
            }
        }
        freeUser(fri);
    } else response_state(client, FAILURE);
}

void handle_msg_send(int client, cJSON* cjson)
{
    Message msg;
    msg.msgFromId = cJSON_GetObjectItem(cjson, "sender")->valueint;
    msg.msgToId = cJSON_GetObjectItem(cjson, "target")->valueint;
    msg.msgContent = cJSON_GetObjectItem(cjson, "content")->valuestring;
    msg.msgStatus = 0;
    User* user = getUserInfoById(msg.msgToId, conn);
    /* 如果对方在线 */
    if(user->userStatus == 1)
    {
        int client_2 = conn_to(user->userIp, CLIENT_PORT);

        cJSON* cjson_2 = cJSON_CreateObject();
        cJSON_AddItemToObject(cjson_2, "type", cJSON_CreateNumber(SEND_MESSAGE));
        cJSON_AddItemToObject(cjson_2, "sender", cJSON_CreateNumber(msg.msgFromId));
        cJSON_AddItemToObject(cjson_2, "checked", cJSON_CreateNumber(UNCHECKED));
        cJSON_AddItemToObject(cjson_2, "date", cJSON_CreateString("2019/9/3"));
        cJSON_AddItemToObject(cjson_2, "content", cJSON_CreateString(msg.msgContent));
        send_cjson(client_2, cjson_2);
        cJSON_Delete(cjson_2);
        cJSON* cj_2 = recv_cjson(client_2, NULL, NULL);
        close(client_2);
        if(cj_2 != NULL)
        {
            msg.msgStatus = CHECKED;
            cJSON_Delete(cj_2);
        }
    }
    insertMsg(&msg, conn);
    freeUser(user);
    response_state(client, SUCCESS);
}

void handle_file_send(int client, cJSON* cjson)
{
    Message msg;
    msg.msgFromId = cJSON_GetObjectItem(cjson, "origin")->valueint;;
    msg.msgToId = cJSON_GetObjectItem(cjson, "target")->valueint;
    msg.msgContent = cJSON_GetObjectItem(cjson, "name")->valuestring;
    msg.msgStatus = 0;
    int size = cJSON_GetObjectItem(cjson, "size")->valueint;
    char* name = msg.msgContent;
    state type = recv_file(client, size, name);
    /* send to server */
    if(msg.msgToId == 0) response_state(client, type);
    else if(type == SUCCESS)
    {
        User* user = getUserInfoById(msg.msgToId, conn);
        if(user->userStatus == 1)
        {
            int client_2 = conn_to(user->userIp, CLIENT_PORT);
            send_cjson(client_2, cjson);
            cJSON* cj_2 = recv_cjson(client_2, NULL, NULL);
            if(cj_2 != NULL)
            {
                send_file(client_2, name, size, NULL);
                cJSON_Delete(cj_2);
            }
            msg.msgStatus = 1;
        }
        freeUser(user);
    }
    insertMsg(&msg, conn);
}

void handle_cjson(int client, cJSON* cjson)
{
    cJSON* type = cJSON_GetObjectItem(cjson, "type");
    void (*handle)(int, cJSON*);
    switch(type->valueint)
    {
        case REGIST: handle = handle_regist_request; break;
        case LOGIN: handle = handle_login_request; break;
        case LOGOUT: handle = handle_logout_request; break;
        case SEND_MESSAGE: handle = handle_msg_send; break;
        case SEND_FILE: handle = handle_file_send; break;
        case REQUEST_FRIEND_LIST: handle = handle_friend_list_request; break;
        case REQUEST_MESSAGE_LIST: handle = handle_msg_list_request; break;
        case ADD_FRIEND_REQUEST: handle = handle_add_friend_request; break;
        case REQUEST_GROUP_LIST: handle = handle_group_list_request; break;
        case REQUEST_GROUP_MESSAGE_LIST: handle = handle_group_message_list_request; break;
        case REQUEST_GROUP_MEMBER_LIST: handle = handle_group_member_list_request; break;
        case CREATE_GROUP_REQUEST: handle = handle_create_group_request; break;
        case JOIN_GROUP_REQUEST: handle = handle_join_group_request; break;
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
    conn = mysqlConnection();
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