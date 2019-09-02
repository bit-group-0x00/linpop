//
// Created by Anne Wu on 2019-09-02.
//

#include "../include/UI_interface.h"
#include "../include/NET_client.h"


state regist(const char* nick_name, const char* passwd)
{
    return 12345;
}

state login(const int id, const char* passwd)
{
   return SUCCESS;
}


int main(int argc, char* argv[])
{
    login_window();
//    homepage_window(12345);
    return 0;
}

