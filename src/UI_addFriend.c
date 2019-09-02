//
// Created by Anne Wu on 2019-08-30.
//
#include "../include/UI_login.h"
#include "../include/NET_client.h"

GtkWidget *SearchResultBox;
GtkWidget *SearchResultImg;
GtkWidget *SearchResultNickname;
GtkWidget *SearchResultID;
GtkWidget *SearchResultLabel;

GtkWidget *AddButton;

gint callback_result = -2;
gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_main_quit();
    return FALSE;
}
void enter_callback(GtkWidget *widget,GtkWidget *entry)
{
    const gchar *entry_text;//好友ID
    entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
    /*ifExisted   not --retrun -1 不存在该用户
     *
     */
    /*ifAdded   --retrun 0 已经添加好友
     * not ---return 1  存在该用户可以添加好友
     *
     */
   // GtkWidget *image = gtk_image_new_from_file("../res/icon.png");
    //gtk_image_set_from_image(GTK_IMAGE(SearchResultImg),GTK_IMAGE(image),NULL);

    callback_result = 1;
    const gchar *Image_address = "../res/icon.png";
    const gchar *ID_char = "ID： 1111111";
    const gchar *Nickname_char = "NickName: yuanyuanyuan";
    GdkPixbuf *Image_buf = gdk_pixbuf_new_from_file(Image_address,NULL);



    SearchResultImg = gtk_image_new_from_file("../res/icon.png");
    gtk_box_pack_start(GTK_BOX(SearchResultBox),SearchResultImg,FALSE,FALSE,10);


    SearchResultID = gtk_label_new("ID: 1111111111");
    gtk_label_set_width_chars(GTK_LABEL(SearchResultID),0);
    gtk_box_pack_start(GTK_BOX(SearchResultBox),SearchResultLabel,FALSE,FALSE,10);

    SearchResultNickname = gtk_label_new("NickName: yuanyuanyuan");
    gtk_label_set_width_chars(GTK_LABEL(SearchResultNickname),0);
    gtk_box_pack_start(GTK_BOX(SearchResultBox),SearchResultNickname,FALSE,FALSE,10);


    if(callback_result == -1)
    {
        gtk_widget_hide(SearchResultNickname);
        gtk_widget_hide(SearchResultID);
        gtk_widget_hide(SearchResultImg);

        gtk_label_set_text(SearchResultLabel,"User not Exist");
        gtk_widget_show(SearchResultLabel);
    }
    else if(callback_result == 0)
    {
        gtk_label_set_text(SearchResultLabel,"Already your Friend");
        gtk_widget_show(SearchResultLabel);
        gtk_image_set_from_pixbuf(GTK_IMAGE(SearchResultImg),Image_buf);
        gtk_widget_show(SearchResultImg);
        gtk_label_set_text(SearchResultID,ID_char);
        gtk_widget_show(SearchResultID);
        gtk_label_set_text(SearchResultNickname,Nickname_char);
        gtk_widget_show(SearchResultNickname);

    }
    else if(callback_result == 1)
    {
        gtk_widget_hide(SearchResultLabel);
        gtk_image_set_from_pixbuf(GTK_IMAGE(SearchResultImg),Image_buf);
        gtk_widget_show(SearchResultImg);
        gtk_label_set_text(SearchResultID,ID_char);
        gtk_widget_show(SearchResultID);
        gtk_label_set_text(SearchResultNickname,Nickname_char);
        gtk_widget_show(SearchResultNickname);

        gtk_widget_set_sensitive(AddButton,TRUE);

    }


    printf("Entry content: %s \n",entry_text);

}


void addFriend(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *vBox;

    GtkWidget *FriendInfoBox;
    GtkWidget *FriendnameLabel;
    GtkWidget *Entry_ID;
    GtkWidget *SearchButton;


    GtkWidget *OptionBox;

    GtkWidget *CancelButton;

    gtk_init (&argc,&argv);
    //window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Add Friend");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_widget_set_size_request(window,400,300);
    gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(delete_event),NULL);

//big box
    vBox = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),vBox);
    gtk_widget_show(vBox);

//FriendInfoBox
    FriendInfoBox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vBox),FriendInfoBox,FALSE,FALSE,30);
    //InfoText
    FriendnameLabel = gtk_label_new("Friend ID: ");
    gtk_label_set_width_chars(FriendnameLabel,10);
    gtk_box_pack_start(GTK_BOX(FriendInfoBox),FriendnameLabel,FALSE,FALSE,0);


    //entry ID
    Entry_ID = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(FriendInfoBox),Entry_ID,FALSE,FALSE,0);
    gtk_entry_set_max_length(GTK_ENTRY(Entry_ID),20);
    g_signal_connect(G_OBJECT(Entry_ID),"activate",G_CALLBACK(enter_callback),Entry_ID);

    //SearchButton
    SearchButton = gtk_button_new_with_label("Search");
    gtk_box_pack_start(GTK_BOX(FriendInfoBox),SearchButton,FALSE,FALSE,0);

    g_signal_connect(G_OBJECT(SearchButton),"clicked",G_CALLBACK(enter_callback),Entry_ID);

//SearchResultBox
    SearchResultBox = gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vBox),SearchResultBox,FALSE,FALSE,0);
    //SearchResultLabel
    SearchResultLabel = gtk_label_new("Click Search Button ");
    gtk_box_pack_start(GTK_BOX(SearchResultBox),SearchResultLabel,FALSE,FALSE,0);
    //SearchResultImg

    //SearchResultID

   // gtk_widget_hide(SearchResultID);
    //SearchResultNickName








    //

//optionBox
    OptionBox = gtk_hbox_new(TRUE,0);
    gtk_box_pack_end(GTK_BOX(vBox),OptionBox,FALSE,FALSE,10);
    //AddButton
    AddButton = gtk_button_new_with_label("AddButton");
    gtk_box_pack_start(GTK_BOX(OptionBox),AddButton,FALSE,FALSE,0);
    gtk_widget_set_sensitive(AddButton,FALSE);
    g_signal_connect(G_OBJECT(AddButton),"clicked",)
    //g_signal_connect(G_OBJECT(AddButton),"clicked",G_CALLBACK())
    //CancelButton
    CancelButton = gtk_button_new_with_label("CancelButton");
    gtk_box_pack_start(GTK_BOX(OptionBox),CancelButton,FALSE,FALSE,0);
    g_signal_connect(G_OBJECT(CancelButton),"clicked",G_CALLBACK(delete_event),NULL);




    gtk_widget_show(window);













    gtk_widget_show_all(window);
    gtk_main();


}


