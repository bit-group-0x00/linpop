//
// Created by Anne Wu on 2019-09-02.
//

#include "../include/UI_interface.h"
#include "../include/NET_client.h"

state request_my_info(info* my_info){
    my_info->my_profile.nick_name = "penguin";
    my_info->my_profile.avatar = "../res/icon";


};

state request_user_profile(const int user_id, void(*callback)(state, profile)){

};

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
    return 0;
}

