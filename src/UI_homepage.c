//
// Created by Anne Wu on 2019-08-30.
//

#include "../include/UI_interface.h"
#include "../include/NET_client.h"
//#include "../include/NET_client.h"
static gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data){
    g_print("delete event occured\n");
    gtk_main_quit();
//    log_out(userID);
    return FALSE;
}

GtkWidget *user_profile(GtkWidget *parent,profile *userInfo, int type, int avaterSize){
    GtkWidget *hbox = gtk_hbox_new(FALSE,0);
    GtkWidget *vbox = gtk_vbox_new(FALSE,0);

    GdkPixbuf *avaterRes = gdk_pixbuf_new_from_file_at_size(userInfo->avatar,avaterSize,avaterSize,NULL);
    GtkWidget *avaterImage = gtk_image_new_from_pixbuf(avaterRes);
    GtkWidget *nickname = gtk_label_new(userInfo->nick_name);

    gtk_container_add(GTK_BOX(hbox),avaterImage);
    gtk_container_add(GTK_BOX(hbox),vbox);
    gtk_container_add(GTK_BOX(vbox),nickname);

    GtkWidget *IPaddress;
    GtkWidget *ID;
    GtkWidget *signature;


    if(type == FRIEND){
        IPaddress = gtk_label_new(g_strdup_printf("%d",userInfo->IPaddress));
        ID = gtk_label_new(g_strdup_printf("%d",userInfo->id));
        signature = gtk_label_new(userInfo->signature);
        if(userInfo->online == ONLINE){
            gtk_label_set_markup(GTK_LABEL(nickname), "<span foreground='blue' font_desc='16'>label for test!</span>");

        }

    }
    else{

    }



}

void homepage_window(int userID){
    //主窗口
    g_print("Succesful login\n");
    //主窗口初始化
    info *userInfo ;
    userInfo->my_profile.id = userID;
    request_my_info(userInfo);

    GtkWidget *homepageWindow;
    homepageWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(homepageWindow),"delete_event",G_CALLBACK(delete_event),NULL);
    gtk_window_set_title(GTK_WINDOW(homepageWindow),userInfo->my_profile.nick_name);
    gtk_window_set_position(GTK_WINDOW(homepageWindow),GTK_WIN_POS_CENTER);

    //主窗口垂直盒
    GtkWidget *homepageBox = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(homepageWindow),homepageBox);

    //用户个人信息和好友列表上下分栏
    GtkWidget *homepagePaned = gtk_vpaned_new();
    gtk_container_add(GTK_CONTAINER(homepageBox),homepagePaned);

    //用户个人界面面板
//    GtkWidget *userInfoPaned = gtk_;


    //好友界面面板
    GtkWidget *friendListViewPort;
    GtkWidget *friendListScrolledWindow;

    //添加按钮盒，设置按钮盒外观
    GtkWidget *addButtonBox = gtk_hbutton_box_new();
    gtk_container_add(GTK_CONTAINER(homepageBox),addButtonBox);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(addButtonBox),GTK_BUTTONBOX_CENTER);
    gtk_box_set_spacing(GTK_BUTTON_BOX(addButtonBox),5);

    //添加好友按钮
    GtkWidget *addFriendButton = gtk_button_new_from_stock(GTK_STOCK_ADD);
    gtk_button_set_label(GTK_BUTTON(addFriendButton),"Friend");
    //添加群聊按钮
    GtkWidget *addGroupChatButton = gtk_button_new_from_stock(GTK_STOCK_ADD);
    gtk_button_set_label(GTK_BUTTON(addGroupChatButton),"Group Chat");

    GtkWidget *avatarImage;
//    GtkWidget *avatarRes;




    homepageWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_get_screen(homepageWindow);
    gtk_window_set_title(GTK_WINDOW(homepageWindow),userInfo->my_profile.nick_name);
    gtk_window_set_position(GTK_WINDOW(homepageWindow),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(homepageWindow),360,640);
    gtk_container_set_border_width(GTK_CONTAINER(homepageWindow),20);

    //    //Big box：最外层box
//    box = gtk_vbox_new(FALSE,0);
//    gtk_container_add(GTK_CONTAINER(loginWindow),box);
//
//    //原始icon图标
//    iconImageRes = gdk_pixbuf_new_from_file_at_size("../res/icon.png",ICON_SIZE,ICON_SIZE,NULL);
//    image = gtk_image_new_from_pixbuf(iconImageRes);
//    gtk_box_pack_start(GTK_BOX(box),image,FALSE,FALSE,5);
//
//    //input box
//    infoBox = gtk_vbox_new(FALSE,0);
//    gtk_box_pack_start(GTK_BOX(box),infoBox,FALSE,FALSE,5);
//
//    usernameBox = gtk_hbox_new(FALSE,0);
//    gtk_box_pack_start(GTK_BOX(infoBox),usernameBox,FALSE,FALSE,0);
//
//    passwordBox = gtk_hbox_new(FALSE,0);
//    gtk_box_pack_start(GTK_BOX(infoBox),passwordBox,FALSE,FALSE,0);
//
//    usernameLable = gtk_label_new("Login ID:");
//    gtk_label_set_width_chars(usernameLable,15);
//    gtk_box_pack_start(GTK_BOX(usernameBox),usernameLable,FALSE,FALSE,5);
//    usernameText = gtk_entry_new();
//    gtk_entry_set_max_length(usernameText,20);
//    gtk_entry_set_text(GTK_ENTRY(usernameText),"12345");
//    gtk_box_pack_start(GTK_BOX(usernameBox),usernameText,FALSE,FALSE,5);
//
//    passwordLable = gtk_label_new("Password:");
//    gtk_label_set_width_chars(passwordLable,15);
//    gtk_box_pack_start(GTK_BOX(passwordBox),passwordLable,FALSE,FALSE,5);
//
//    passwordText = gtk_entry_new();
//    gtk_entry_set_visibility(GTK_ENTRY(passwordText),FALSE);
//    gtk_entry_set_text(GTK_ENTRY(passwordText),"wqxpassword");
//    gtk_entry_set_max_length(passwordText,20);
//    gtk_box_pack_start(GTK_BOX(passwordBox),passwordText,FALSE,FALSE,5);
//
//    sep = gtk_hseparator_new();
//    gtk_box_pack_start(GTK_BOX(infoBox),sep,FALSE,FALSE,5);
//
//    //button box
//    optionBox = gtk_hbutton_box_new();
//    gtk_box_pack_start(GTK_BOX(box),optionBox,FALSE,FALSE,5);
//
//    registButton = gtk_button_new_with_label("Don't have an account?");
//    g_signal_connect(G_OBJECT(registButton),"clicked",G_CALLBACK(on_button_clicked),(gpointer)REGIST);
//    g_signal_connect_swapped(G_OBJECT(registButton),"clicked",G_CALLBACK(gtk_widget_hide),loginWindow);
//    gtk_container_add(GTK_CONTAINER(optionBox),registButton);
//
//    logButton = gtk_button_new_with_label("Log in");
//    g_signal_connect(G_OBJECT(logButton),"clicked",G_CALLBACK(on_button_clicked),(gpointer)LOG_IN);
//    g_signal_connect_swapped(G_OBJECT(logButton),"clicked",G_CALLBACK(gtk_widget_destroy),loginWindow);
//    gtk_container_add(GTK_CONTAINER(optionBox),logButton);
//
//    g_signal_connect(G_OBJECT(loginWindow),"delete_event",G_CALLBACK(delete_event), NULL);
//
//
//    gtk_widget_show_all(loginWindow);
//
//    gtk_main();
//


}