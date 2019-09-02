/*
  File Name: NET_client.h
  Model Name: NET
  Create Date: 2019/8/29
  Author: PengYao
  Abstract Description: clent side program
*/

#include "NET_macro.h"
#include "NET_socket.h"
#include "UTIL_cJSON.h"

/* 我的账号、当前账号 */
extern int my_id;

/*
  存放用户简要信息的结构体，用于向其他用户展示
  和显示在自己的信息栏
*/
typedef struct profile
{
  int id;
  char* nick_name;
  char* avatar;
} profile;
/* 消息结构体，用双向链表表示消息队列 */
typedef struct message
{
  int sender;
  char* msg;
  message* last;
  message* next;
} message;
/* 好友结构体，包含好友的简要信息和聊天消息 */
typedef struct friend
{
  profile friend_profile;
  message msg;
} friend;

/* 群聊结构体，包括群聊的基本信息和聊天消息 */
typedef struct group
{
  profile group_profile;
  message msg;
}

/*
  有关我的详细信息，所有界面要显示的信息
  都应该放在这个结构体里面
*/
typedef struct info
{
  profile my_profile;
  int friend_num;
  friend* friends;
  int group_num;
  group* groups;
} info;

/* 
  client regist fuction
  传入用户注册的昵称和密码，如果注册成功返回注册成功后的账号id
  如果注册失败返回FAILURE（1），如果注册过错产生错误返回ERROR（-1）
  注：-1，0，1都是系统保留值，不能作为账号
*/
state regist(const char* nick_name, const char* passwd);

/* 
  client login function
  传入用户的账号和密码，如果登陆成功返回SUCCESS（0），如果失败
  返回FAILURE（1），如果登陆过程中发生了错误返回ERROR（-1）
*/
state login(const int id, const char* passwd);

/*
  request my info
  请求自己的详细信息，传入存放详细信息的结构体指针，返回值表示
  请求的结果SUCCESS（0）表示成功，FAILURE（1）表示失败，
  ERROR（-1）表示请求过程中发生了错误。最后的用户数据放在my_info
  指向的结构体里面。
*/
state request_my_info(info* my_info);

/*
  request user's profile
  请求用户的简要信息，区别于上面的的获取自己的详细信息，
  这里的这个函数是获得好友的简要信息，用于列表显示。
  传入用户的id和回调函数，返回SUCCESS（0）表示正在请求，
  FAILURE（1）表示请求失败。
  回调函数的作用：在调用请求用户信息的函数后，由于要和服务端
  网络传输可能会堵塞主线程，为了避免这种情况发生，这里会创建一个
  新的线程来执行请求，新的线程执行结束之后调用回调函数，在回调
  函数里面执行更新界面操作。
  回调函数的参数有state和profile，其中state用于表示请求的
  结果，用SUCCESS（0）表示请求信息成功，FAILURE（1）表示
  请求信息失败，ERROR（-1）表示请求过程中发生了错误；profile
  结构体表示具体请求到的信息，用于更新界面。
*/
state request_user_profile(const int user_id, void(*callback)(state, profile));

/*
  send message to friend
  实现发送消息至某个好友，传入好友账号、要发送的消息和回调函数。
  说明下回调函数的作用：由于发送过程可能很长，而用户在发送消息
  的同时可能要去做其他的事情，所以在这里传入一个回调函数，当消息
  发送完成后（不一定成功）调用该函数，告诉用户发送消息的结果。
  这里的返回值不能作为发送消息是否成功的参考，真正的成功和失败
  会作为参数传给回调函数，成功传入SUCCESS（0），失败传入FAILURE(1)
  传输过错发生错误传入（-1）。回调函数的原型为void(*)(state)。
  下面的函数先检查是否满足发送消息的条件，如果不满足，直接返回FAILURE(1),
  如果满足，则返回SUCCESS（0），表示消息正在发送中。消息发送完成后
  调用回调函数，并传入发送结果。
*/
state send_msg_to_friend(const int friend_id, const char* msg, void(*callback)(state));

/* 
  send message to a group 
  向群聊发送消息，传入群聊ID，要发送的消息和回调函数。
  这里的回调函数作用和上面的函数一致，具体的行为不同。
  这个函数先检查是否满足向群聊发送消息的条件，如果不
  满足直接返回FAILURE（1），如果满足，返回SUCCESS（0），
  表示消息正在发送中。消息发送完成后调用回调函数并传入
  发送结果，SUCCESS（0）表示发送成功，FAILURE（1）表示
  发送失败，ERROR（-1）表示发送过程中出现了错误。
*/
state send_msg_to_group(const int group_id, const char* msg, void(*callback)(state));

/*
  向朋友发送文件，功能类似于向朋友发送消息，这里把文件的路径
  作为参数
*/
state send_file_to_friend(const int friend_id, const char* file_path, void(*callback)(state));

/*
  向群聊发送文件，功能类似于向朋友发送消息，这里把文件的路径
  作为参数
*/
state send_file_to_group(const int group_id, const char* file_path, void(*callback)(state));