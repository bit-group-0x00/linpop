//
// Created by Anne Wu on 2019-09-02.
//

#include "../include/UI_interface.h"
#include "../include/NET_client.h"
#include "../include/UI_chatView.h"

GtkWidget *user_profile_test(profile userInfoDisplay, int type, int avaterSize){
    GtkWidget *hbox = gtk_hbox_new(FALSE,0);
    GtkWidget *vbox = gtk_vbox_new(FALSE,0);

    GdkPixbuf *avaterRes = gdk_pixbuf_new_from_file_at_size(userInfoDisplay.avatar,avaterSize,avaterSize,NULL);
    GtkWidget *avaterImage = gtk_image_new_from_pixbuf(avaterRes);
    GtkWidget *nickname = gtk_label_new(userInfoDisplay.nick_name);
    gtk_box_pack_start(GTK_BOX(hbox),avaterImage,TRUE,TRUE,5);
    gtk_box_pack_start(GTK_BOX(hbox),vbox,TRUE,TRUE,5);
    gtk_box_pack_start(GTK_BOX(vbox),nickname,TRUE,TRUE,5);
    GtkWidget *signature = gtk_label_new(userInfoDisplay.signature);
    label_font(signature,userInfoDisplay.signature,FONT_SIZE_SMALL,"#35333C","low","blue");
    gtk_box_pack_end(GTK_BOX(vbox),signature,TRUE,TRUE,5);
    if(type == GROUP){
        //群聊
        label_font(nickname,userInfoDisplay.nick_name,FONT_SIZE_MIDDLE,"black","none","blue");
    }
    else {
        GtkWidget *IPaddress = gtk_label_new(NULL);
        GtkWidget *ID = gtk_label_new(NULL);
        label_font(IPaddress,userInfoDisplay.ip,10,"#213174","single","black");
        label_font(ID,g_strdup_printf("%d",userInfoDisplay.id),10,"#213174","single","black");
        if(type == FRIEND){
            if(userInfoDisplay.online == ONLINE){
                //在线好友
                label_font(nickname,userInfoDisplay.nick_name,FONT_SIZE_MIDDLE,"#C8ADC4","double","blue");
            }
            else{
                //不在线好友
                label_font(nickname,userInfoDisplay.nick_name,FONT_SIZE_MIDDLE,"black","single","black");
            }
        }
        else{
            //个人
            label_font(nickname,userInfoDisplay.nick_name,FONT_SIZE_BIG,"#61649F","none","blue");
        }
        gtk_box_pack_start(GTK_BOX(vbox),IPaddress,TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(vbox),ID,TRUE,TRUE,0);
    }
    return hbox;
}

state regist(const char* nick_name, const char* passwd)
{
    return 12345;
}

state login(const int id, const char* passwd)
{
   return SUCCESS;
}

int main(){
    login_window();
//    homepage_window(1234);
}