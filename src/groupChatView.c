//
// Created by yuanyuanyaun on 9/2/19.
//
#include "../include/UI_interface.h"
#include "../include/NET_client.h"

void group_chat_window(int argc, char* argcv[])
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
    GtkWidget *listbox;
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

    listbox = gtk_list_box_new();

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


