/*
  File Name: NET_macro.h
  Model Name: NET
  Create Date: 2019/8/29
  Author: PengYao
  Abstract Description: macro
*/

#define SUCCESS 0
#define FAILURE 1
#define ERROR -1

#define REGIST 0X010
#define LOGIN 0X020
#define LOGOUT 0X030
#define SEND_MESSAGE 0X040
#define SEND_FILE 0X050
#define REQUEST_FRIEND_LIST 0x060
#define REQUEST_MESSAGE_LIST 0x070
#define ADD_FRIEND_REQUEST 0x080
#define CHECKED 0
#define UNCHECKED 1
#define ONLINE 1
#define OFFLINE 0
#define REQUEST_GROUP_LIST 0x400
#define REQUEST_GROUP_MESSAGE_LIST 0x500
#define REQUEST_GROUP_MEMBER_LIST 0x600
#define CREATE_GROUP_REQUEST 0X700
#define JOIN_GROUP_REQUEST 0x800
#define SEND_MESSAGE_TO_GROUP 0X900

typedef int state;
//typedef unsigned long ip;
