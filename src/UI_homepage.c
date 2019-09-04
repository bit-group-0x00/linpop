//
// Created by Anne Wu on 2019-08-30.
//

#include "../include/UI_interface.h"
#include "../include/NET_client.h"

GtkWidget *homepageWindow;
static GdkPixbuf *imageRes;

static info userinfo;

static gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data){
    gint state;
    g_print("delete event occured\n");
    if(show_question(widget,NULL,"do you really want to exit?") == GTK_RESPONSE_YES){
//        log_out(userID);
        gtk_main_quit();
        return FALSE;
    }
    else{
        return TRUE;
    }
}
void label_font(GtkWidget *label, gchar* context, int fontSize, gchar *foreColor, gchar *underline, gchar *underlineColor){
    gtk_label_set_markup(GTK_LABEL(label), g_strdup_printf("%s%s%s%s%s%s%s%d%s%s%s","<span foreground='",foreColor,"' underline='",underline,"' underline_color='",underlineColor,"' font_desc='",fontSize,"'>",context,"</span>"));
    gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_LEFT);
}
GtkWidget *create_userbox(profile userinfoDisplay, int type, int avaterSize){
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);

    GdkPixbuf *avaterRes = gdk_pixbuf_new_from_file_at_size(userinfoDisplay.avatar,avaterSize,avaterSize,NULL);
    GtkWidget *avaterImage = gtk_image_new_from_pixbuf(avaterRes);
    GtkWidget *nickname = gtk_label_new(userinfoDisplay.nick_name);
    GtkWidget *signature = gtk_label_new(userinfoDisplay.signature);
    gint expand,fill;
    expand = FALSE;
    fill = FALSE;

    GtkWidget *nicknamebox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    gtk_box_pack_start(GTK_BOX(nicknamebox),nickname,expand,fill,5);
    GtkWidget *signaturebox= gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    gtk_box_pack_start(GTK_BOX(signaturebox),signature,expand,fill,5);


    gtk_box_set_homogeneous (GTK_BOX(hbox),FALSE);
    label_font(signature,userinfoDisplay.signature,FONT_SIZE_SMALL,"#35333C","none","blue");
    gtk_box_pack_start(GTK_BOX(hbox),avaterImage,expand,fill,5);
    gtk_box_pack_start(GTK_BOX(hbox),vbox,TRUE,TRUE,5);
    gtk_box_pack_start(GTK_BOX(vbox),nicknamebox,expand,fill,5);
    gtk_box_pack_end(GTK_BOX(vbox),signaturebox,expand,fill,5);
    if(type == GROUP){
        //群聊
        label_font(nickname,userinfoDisplay.nick_name,FONT_SIZE_MIDDLE,"black","none","blue");
        gtk_widget_set_size_request(GTK_WIDGET(avaterImage),100,avaterSize);
    }
    else {
        GtkWidget *ip = gtk_label_new(NULL);
        GtkWidget *ID = gtk_label_new(NULL);
        GtkWidget *ipbox= gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        gtk_box_pack_start(GTK_BOX(ipbox),ip,expand,fill,5);
        GtkWidget *idbox= gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        gtk_box_pack_start(GTK_BOX(idbox),ID,expand,fill,5);
        label_font(ip,userinfoDisplay.ip,10,"#213174","single","black");
        label_font(ID,g_strdup_printf("%d",userinfoDisplay.id),10,"#213174","none","black");
        if(type == FRIEND){
            gtk_widget_set_size_request(GTK_WIDGET(avaterImage),100,avaterSize);
            if(userinfoDisplay.online == ONLINE){
                //在线好友
                label_font(nickname,userinfoDisplay.nick_name,FONT_SIZE_MIDDLE,"#C081AF","none","blue");
            }
            else{
                //不在线好友
                label_font(nickname,userinfoDisplay.nick_name,FONT_SIZE_MIDDLE,"#C8ADC4","none","black");
            }
        }
        else{
            //个人
            label_font(nickname,userinfoDisplay.nick_name,FONT_SIZE_BIG,"#61649F","none","blue");
            gtk_widget_set_size_request(GTK_WIDGET(avaterImage),100,avaterSize);

        }
        gtk_box_pack_start(GTK_BOX(vbox),idbox,expand,fill,0);
        gtk_box_pack_start(GTK_BOX(vbox),ipbox,expand,fill,0);

    }
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(box),hbox,expand,fill,0);
    gtk_box_pack_start(GTK_BOX(box),sep,expand,fill,0);
    return box;
}
void open_chat(GtkWidget *widget,gpointer data){
    g_print("ROW_SELECTED\n");
    GtkListBoxRow *select = gtk_list_box_get_selected_row(GTK_LIST_BOX(widget));
    int index = gtk_list_box_row_get_index(select);
    friend* p = userinfo.first_fri;
    for (int i = 0; p != NULL && i < index; ++i) {
        p = p->next;
    }
    g_print("%d",index);
    group_chat_window(userinfo.my_pro.id,3242342);
//    friend_chat_window(userinfo.my_pro.id,432123);

}

void homepage_window(const int userID){
    //主窗口
    g_print("Succesful login\n");
    //主窗口初始化
    userinfo.my_pro.id = userID;
    userinfo.my_pro.nick_name = "penguin";
    userinfo.my_pro.avatar = "../res/icon.png";
    userinfo.my_pro.online = ONLINE;
    userinfo.my_pro.id = 12345;
    userinfo.my_pro.ip="123.123";
    userinfo.my_pro.signature="Hello world";

    info friendInfo1;
    friendInfo1.my_pro.id = userID;
    friendInfo1.my_pro.nick_name = "penguin1";
    friendInfo1.my_pro.avatar = "../res/avatar_xmas.png";
    friendInfo1.my_pro.online = ONLINE;
    friendInfo1.my_pro.id = 12346;
    friendInfo1.my_pro.ip="123.123.433";
    friendInfo1.my_pro.signature="I am penguin1";

    info friendInfo2;
    friendInfo2.my_pro.id = userID;
    friendInfo2.my_pro.nick_name = "penguin2";
    friendInfo2.my_pro.avatar = "../res/avatar_penguin.png";
    friendInfo2.my_pro.online = OFFLINE;
    friendInfo2.my_pro.id = 12347;
    friendInfo2.my_pro.ip="123.123.312";
    friendInfo2.my_pro.signature="I am penguin2";

    info friendInfo3;
    friendInfo3.my_pro.id = userID;
    friendInfo3.my_pro.nick_name = "penguin3";
    friendInfo3.my_pro.avatar = "../res/icon_color.png";
    friendInfo3.my_pro.online = ONLINE;
    friendInfo3.my_pro.id = 12348;
    friendInfo3.my_pro.ip="123.312.413";
    friendInfo3.my_pro.signature="I am penguin3";

//    info friendInfo4;
//    friendInfo3.my_pro.id = userID;
//    friendInfo3.my_pro.nick_name = "penguin_family";
//    friendInfo3.my_pro.avatar = "../res/icon_people.png";
//    friendInfo3.my_pro.online = ONLINE;
//    friendInfo3.my_pro.id = 123;
//    friendInfo3.my_pro.ip="123.123";
//    friendInfo3.my_pro.signature="This is penguin family";
//
//    info friendInfo5;
//    friendInfo5.my_pro.id = userID;
//    friendInfo5.my_pro.nick_name = "penguin4";
//    friendInfo5.my_pro.avatar = "../res/icons_more_info.png";
//    friendInfo5.my_pro.online = ONLINE;
//    friendInfo5.my_pro.id = 12345;
//    friendInfo5.my_pro.ip="123.21313";
//    friendInfo5.my_pro.signature="I am penguin4";
//
//    info friendInfo6;
//    friendInfo6.my_pro.id = userID;
//    friendInfo6.my_pro.nick_name = "penguin5";
//    friendInfo6.my_pro.avatar = "../res/icons8_error.png";
//    friendInfo6.my_pro.online = ONLINE;
//    friendInfo6.my_pro.id = 12345;
//    friendInfo6.my_pro.ip="123.113131313";
//    friendInfo6.my_pro.signature="I am penguin5";

    homepageWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(homepageWindow),userinfo.my_pro.nick_name);
    gtk_window_set_position(GTK_WINDOW(homepageWindow),GTK_WIN_POS_CENTER);
    gtk_widget_get_screen(homepageWindow);
    gtk_window_set_default_size(GTK_WINDOW(homepageWindow),360,640);
    gtk_container_set_border_width(GTK_CONTAINER(homepageWindow),20);
    gtk_widget_set_opacity(GTK_WIDGET(homepageWindow),0.7);

    //主窗口垂直盒
    GtkWidget *homepageBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    gtk_container_add(GTK_CONTAINER(homepageWindow),homepageBox);

    //添加按钮盒，设置按钮盒外观
    GtkWidget *addButtonBox = gtk_hbutton_box_new();
    gtk_button_box_set_layout(GTK_BUTTON_BOX(addButtonBox),GTK_BUTTONBOX_CENTER);
    gtk_container_add(GTK_CONTAINER(homepageBox),addButtonBox);
    //添加好友按钮
    GtkWidget *addFriendButton = gtk_button_new_with_label("Add Friend");
    gtk_button_set_always_show_image(GTK_BUTTON(addFriendButton),TRUE);
    imageRes = gdk_pixbuf_new_from_file_at_size("../res/icons8-add-user-group-man-man-64.png",30,30,NULL);
    g_signal_connect(GTK_WIDGET(addFriendButton),"clicked",G_CALLBACK(add_friend_window),NULL);
    GtkWidget *addFriendImage = gtk_image_new_from_pixbuf(imageRes);
    gtk_button_set_image(GTK_BUTTON(addFriendButton),addFriendImage);
    gtk_box_pack_start(GTK_BOX(addButtonBox),addFriendButton,FALSE,FALSE,5);

    //添加群聊按钮
    GtkWidget *addGroupChatButton = gtk_button_new_with_label("Add Group");
    gtk_button_set_always_show_image(GTK_BUTTON(addGroupChatButton),TRUE);
    imageRes = gdk_pixbuf_new_from_file_at_size("../res/icons8_people.png",30,30,NULL);
    g_signal_connect(GTK_WIDGET(addGroupChatButton),"clicked",G_CALLBACK(create_group_window), NULL);
    GtkWidget *addGroupImage = gtk_image_new_from_pixbuf(imageRes);
    gtk_button_set_image(GTK_BUTTON(addGroupChatButton),addGroupImage);
    gtk_box_pack_end(GTK_BOX(addButtonBox),addGroupChatButton,FALSE,FALSE,5);

    //用户个人信息和好友列表上下分栏
//    GtkWidget *homepagePaned = gtk_vpaned_new();
//    gtk_widget_set_size_request(GTK_WIDGET(homepagePaned),360,600);
//    gtk_box_pack_start(GTK_BOX(homepageBox),homepagePaned,FALSE,FALSE,5);
    GtkWidget *homepagePaned = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    gtk_widget_set_size_request(GTK_WIDGET(homepagePaned),360,600);
    gtk_box_pack_start(GTK_BOX(homepageBox),homepagePaned,FALSE,FALSE,5);

    //用户个人界面面板
    GtkWidget *userinfoPaned = gtk_frame_new("");
    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(userinfoPaned),create_userbox(userinfo.my_pro,USER_SELF,64));
    gtk_container_add(GTK_CONTAINER(userinfoPaned),sep);
    gtk_box_pack_start(GTK_BOX(homepagePaned),userinfoPaned,FALSE,FALSE,5);

    //好友界面面板
    GtkWidget *friendListScrolledWindow = gtk_scrolled_window_new(NULL,NULL);
    gtk_box_pack_start(GTK_BOX(homepagePaned),friendListScrolledWindow,TRUE,TRUE,5);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(friendListScrolledWindow),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(friendListScrolledWindow),
                                         GTK_SHADOW_ETCHED_IN);

    GtkWidget *listbox = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(friendListScrolledWindow),listbox);

    for (int i = 0; i < 50; i+=6) {
        GtkWidget *testbox2 = create_userbox(friendInfo1.my_pro,FRIEND,40);
        GtkWidget *testbox3 = create_userbox(friendInfo2.my_pro,FRIEND,40);
        GtkWidget *testbox4 = create_userbox(friendInfo3.my_pro,GROUP,40);
//        GtkWidget *testbox5 = create_userbox(friendInfo4.my_pro,FRIEND,40);
//        GtkWidget *testbox6 = create_userbox(friendInfo5.my_pro,FRIEND,40);
//        GtkWidget *testbox7 = create_userbox(friendInfo6.my_pro,GROUP,40);
        gtk_list_box_insert(GTK_LIST_BOX(listbox),testbox2,i);
        gtk_list_box_insert(GTK_LIST_BOX(listbox),testbox3,1+i);
        gtk_list_box_insert(GTK_LIST_BOX(listbox),testbox4,2+i);
//        gtk_list_box_insert(GTK_LIST_BOX(listbox),testbox5,i+3);
//        gtk_list_box_insert(GTK_LIST_BOX(listbox),testbox6,4+i);
//        gtk_list_box_insert(GTK_LIST_BOX(listbox),testbox7,5+i);
    }

    g_signal_connect(G_OBJECT(listbox),"row_activated",G_CALLBACK(open_chat), userID);
    gtk_list_box_set_activate_on_single_click(GTK_LIST_BOX(listbox), FALSE);

    gtk_widget_show_all(homepageWindow);

    g_signal_connect(G_OBJECT(homepageWindow),"delete_event",G_CALLBACK(delete_event),NULL);

    gtk_main();

}