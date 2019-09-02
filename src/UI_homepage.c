//
// Created by Anne Wu on 2019-08-30.
//

#include "../include/UI_interface.h"
#include "../include/NET_client.h"
//#include "../include/NET_client.h"
static info userInfo;
static GtkWidget *homepageWindow;
static GdkPixbuf *imageRes;

enum
{
    LIST_ITEM = 0,
    N_COLUMNS
};

static void
init_list(GtkWidget *list)
{

    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkListStore *store;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("List Items",
                                                      renderer, "text", LIST_ITEM, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

    gtk_tree_view_set_model(GTK_TREE_VIEW(list),
                            GTK_TREE_MODEL(store));

    g_object_unref(store);
}

static void add_to_list(GtkWidget *list, const GtkWidget *hbox)
{
    GtkListStore *store;
    GtkTreeIter iter;

    store = GTK_LIST_STORE(gtk_tree_view_get_model
                                   (GTK_TREE_VIEW(list)));

    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, LIST_ITEM, hbox, -1);
}


void  on_changed(GtkWidget *widget, gpointer label)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    char *value;


    if (gtk_tree_selection_get_selected(
            GTK_TREE_SELECTION(widget), &model, &iter)) {

        gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
        gtk_label_set_text(GTK_LABEL(label), value);
        g_free(value);
    }

}
static gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data){
    g_print("delete event occured\n");
    gtk_main_quit();
//    log_out(userID);
    return FALSE;
}
void label_font(GtkWidget *lable, gchar* context, int fontSize, gchar *foreColor, gchar *underline, gchar *underlineColor){
    gtk_label_set_markup(GTK_LABEL(lable), g_strdup_printf("%s%s%s%s%s%s%s%d%s%s%s","<span foreground='",foreColor,"' underline='",underline,"' underline_color='",underlineColor,"' font_desc='",fontSize,"'>",context,"</span>"));
}
GtkWidget *user_profile(profile userInfoDisplay, int type, int avaterSize){
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
        label_font(IPaddress,userInfoDisplay.IPaddress,10,"#213174","single","black");
        label_font(ID,g_strdup_printf("%d",userInfoDisplay.id),10,"#213174","single","black");
        if(type == FRIEND){
            if(userInfoDisplay.online == ONLINE){
                //在线好友
                label_font(nickname,userInfoDisplay.nick_name,FONT_SIZE_MIDDLE,"red","double","blue");
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

void homepage_window(int userID){
    //主窗口
    g_print("Succesful login\n");
    //主窗口初始化
    userInfo.my_profile.id = userID;
    userInfo.my_profile.nick_name = "penguin";
    userInfo.my_profile.avatar = "../res/icon.png";
    userInfo.my_profile.online = ONLINE;
    userInfo.my_profile.id = 12345;
    userInfo.my_profile.IPaddress="123.123";
    userInfo.my_profile.signature="Hello world";

    homepageWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(homepageWindow),userInfo.my_profile.nick_name);
    gtk_window_set_position(GTK_WINDOW(homepageWindow),GTK_WIN_POS_CENTER);
    gtk_widget_get_screen(homepageWindow);
    gtk_window_set_default_size(GTK_WINDOW(homepageWindow),360,640);
    gtk_container_set_border_width(GTK_CONTAINER(homepageWindow),20);
    gtk_window_set_opacity(GTK_WINDOW(homepageWindow),20);

    //主窗口垂直盒
    GtkWidget *homepageBox = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(homepageWindow),homepageBox);

    //添加按钮盒，设置按钮盒外观
    GtkWidget *addButtonBox = gtk_hbutton_box_new();
    gtk_button_box_set_layout(GTK_BUTTON_BOX(addButtonBox),GTK_BUTTONBOX_CENTER);
    gtk_button_box_set_spacing(GTK_BUTTON_BOX(addButtonBox),5);
    gtk_container_add(GTK_CONTAINER(homepageBox),addButtonBox);
    //添加好友按钮
    GtkWidget *addFriendButton = gtk_button_new_with_label("Add Friend");
    imageRes = gdk_pixbuf_new_from_file_at_size("../res/icons_p.png",30,30,NULL);
    GtkWidget *addFriendImage = gtk_image_new_from_pixbuf(imageRes);
    gtk_button_set_image(GTK_BUTTON(addFriendButton),addFriendImage);
    gtk_box_pack_start(GTK_BOX(addButtonBox),addFriendButton,FALSE,FALSE,5);

    //添加群聊按钮
    GtkWidget *addGroupChatButton = gtk_button_new_with_label("Add Group");
    imageRes = gdk_pixbuf_new_from_file_at_size("../res/icon_c.png",30,30,NULL);
    GtkWidget *addGroupImage = gtk_image_new_from_pixbuf(imageRes);
    gtk_button_set_image(GTK_BUTTON(addGroupChatButton),addGroupImage);
    gtk_box_pack_end(GTK_BOX(addButtonBox),addGroupChatButton,FALSE,FALSE,5);

    //用户个人信息和好友列表上下分栏
    GtkWidget *homepagePaned = gtk_vpaned_new();
    gtk_widget_set_size_request(GTK_WIDGET(homepagePaned),360,600);
    gtk_box_pack_start(GTK_BOX(homepageBox),homepagePaned,FALSE,FALSE,5);

    //用户个人界面面板
    GtkWidget *userInfoPaned = gtk_frame_new("");
    GtkWidget *sep = gtk_hseparator_new();
    gtk_container_add(GTK_CONTAINER(userInfoPaned),user_profile(userInfo.my_profile,USER_SELF,64));
    gtk_container_add(GTK_CONTAINER(userInfoPaned),sep);
    gtk_paned_add1(GTK_PANED(homepagePaned),userInfoPaned);

    //好友界面面板
    GtkWidget *friendListScrolledWindow = gtk_scrolled_window_new(NULL,NULL);
    gtk_paned_add2(GTK_PANED(homepagePaned),friendListScrolledWindow);
//    GtkWidget *listBox;
//    GtkTreeSelection *selection;
//    GtkWidget *testbox = user_profile(userInfo.my_profile,FRIEND,30);
//    list = gtk_tree_view_new();
//
//    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(friendListScrolledWindow),
//                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
//    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(friendListScrolledWindow),
//                                         GTK_SHADOW_ETCHED_IN);
//    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (list), FALSE);
//    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (list), FALSE);
//
//    gtk_container_add(GTK_CONTAINER(friendListScrolledWindow),list);
//    init_list(list);
//    add_to_list(list,testbox);
//
//    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

    gtk_widget_show_all(homepageWindow);
    g_signal_connect(G_OBJECT(homepageWindow),"delete_event",G_CALLBACK(delete_event),NULL);
    gtk_main();

}