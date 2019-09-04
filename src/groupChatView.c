//
// Created by yuanyuanyaun on 9/2/19.
//
#include "../include/UI_interface.h"
#include "../include/NET_client.h"



void add_item_list_box(GtkWidget *listbox,gchar*messageSenderIcon,int senderId,gchar*messageSenderName,gchar*message,int type)
{
    GtkWidget *messageBox;
    GtkWidget *SenderIcon;
    GtkWidget *src;
    GtkWidget *dst;

    GtkTextView *text;
    GtkTextBuffer*textBuffer;

    messageBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    src = gdk_pixbuf_new_from_file(messageSenderIcon, NULL);
    dst = gdk_pixbuf_scale_simple(src, 20, 20, GDK_INTERP_BILINEAR);
    SenderIcon = gtk_image_new_from_pixbuf(dst);
    g_object_unref(src);	// pixbuf使用完，需要人为释放资源
    g_object_unref(dst);
    gtk_box_pack_start(GTK_BOX(messageBox),SenderIcon,TRUE,FALSE,0);

    text = gtk_text_view_new();
    textBuffer = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(textBuffer,message,-1);
    gtk_text_view_set_buffer(text,textBuffer);
    gtk_box_pack_start(GTK_BOX(messageBox),text,TRUE,FALSE,0);


   // gtk_text_buffer_set_text()



}


void group_chat_window(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *bigBox;

    GtkWidget *groupInfoBox;
    GtkWidget *groupLabel;
    GtkWidget *groupImg;

    GtkWidget *separator;

    GtkWidget *vPaned;
    GtkWidget *leftFrame;
    GtkWidget *rightFrame;
    //leftFrame
    GtkWidget *hPaned;
    GtkWidget *upFrame;
    GtkWidget *downFrame;
    //upFrame
    GtkWidget *scrolledMessageList;
    GtkWidget *listBox;
    //downFrame
    GtkWidget *toolBar;
    GtkWidget *textView;
    GtkWidget *sendButton;
    GtkWidget *cancelButton;
    //rightFrame
    GtkWidget *vRightBox;
    GtkWidget *groupIntroTextView;
    GtkWidget *rightSeparator;
    GtkWidget *scrolledMemberList;


    //window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(window,"Group Chat Room");
    gtk_widget_set_size_request(window,1092,744);
    g_signal_connect(window,"delete_event",gtk_main_quit,NULL);

    //bigBox
    bigBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    gtk_container_add(GTK_CONTAINER(window),bigBox);

    //groupInfoBox
    groupInfoBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    gtk_box_pack_start(GTK_BOX(bigBox),groupInfoBox,FALSE,FALSE,0);
    //groupImg
    groupImg = gtk_image_new_from_file("../res/icon.png");
    gtk_box_pack_start(GTK_BOX(groupInfoBox),groupImg,FALSE,FALSE,15);
    gtk_image_set_pixel_size(GTK_IMAGE(groupImg),20);
    //groupLabel
    groupLabel = gtk_label_new(NULL);
    const gchar * groupLabelString = "pulalala pupupu <ID: 123456>";
    gtk_label_set_label(GTK_LABEL(groupLabel),groupLabelString);
    gtk_box_pack_start(GTK_BOX(groupInfoBox),groupLabel,FALSE,FALSE,0);
    //separtor
    separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(bigBox),separator,FALSE,FALSE,0);

    //hPaned
    hPaned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_size_request(hPaned,1092,-1);
    gtk_container_add(bigBox,hPaned);

    //leftFrame
    leftFrame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(leftFrame),GTK_SHADOW_IN);
    gtk_widget_set_size_request(leftFrame,822,-1);
    gtk_paned_pack1(GTK_PANED(hPaned),leftFrame,TRUE,TRUE);


    //vPaned
    vPaned = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
    gtk_container_add(GTK_CONTAINER(leftFrame),vPaned);
    //upFrame
    upFrame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(upFrame),GTK_SHADOW_IN);
    gtk_widget_set_size_request(upFrame,-1,354);
    gtk_paned_pack1(GTK_PANED(vPaned),upFrame,TRUE,TRUE);

/*
    GtkWidget *friendListScrolledWindow = gtk_scrolled_window_new(NULL,NULL);
    gtk_box_pack_start(GTK_BOX(homepagePaned),friendListScrolledWindow,TRUE,TRUE,5);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(friendListScrolledWindow),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(friendListScrolledWindow),
                                         GTK_SHADOW_ETCHED_IN);

    GtkWidget *testbox = user_profile(userInfo.my_profile,FRIEND,30);
    GtkWidget *listbox = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(friendListScrolledWindow),listbox);
    gtk_list_box_prepend(GTK_LIST_BOX(listbox),testbox);
   */

    scrolledMessageList = gtk_scrolled_window_new(NULL,NULL);
    gtk_paned_pack1(GTK_PANED(vPaned),scrolledMessageList,TRUE,TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledMessageList),GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledMessageList),GTK_SHADOW_IN);


    listBox = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrolledMessageList),listBox);

   // g_signal_connect(listbox,"selection_changed",G_CALLBACK(listbox_changed),NULL);



    //downFrame
    downFrame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(downFrame),GTK_SHADOW_IN);
    gtk_widget_set_size_request(downFrame,-1,254);
    gtk_paned_pack2(GTK_PANED(vPaned),downFrame,TRUE,TRUE);






    //rightFrame
    rightFrame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(rightFrame),GTK_SHADOW_IN);
    gtk_widget_set_size_request(rightFrame,270,-1);
    gtk_paned_pack2(GTK_PANED(hPaned),rightFrame,TRUE,FALSE);


    gtk_widget_show_all(window);
    gtk_main();




}


