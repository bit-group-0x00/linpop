//
// Created by Anne Wu on 2019-09-02.
//

#include "../include/UI_interface.h"
#include "../include/NET_client.h"
#include "../include/UI_chatView.h"


state regist(const char* nick_name, const char* passwd, const char* signature, const char* avatar)
{
    return 12345;
}

state login(const int id, const char* passwd)
{
   return SUCCESS;
}

int main(){
    login_window();
//    group_chat_window(1234,1235);
//    homepage_window(1234);
}