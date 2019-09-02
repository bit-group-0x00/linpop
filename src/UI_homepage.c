//
// Created by Anne Wu on 2019-08-30.
//

#include "../include/UI_interface.h"
#include "../include/NET_client.h"

void homepage_window(int userID){
    GtkWidget *homepageWindow;
    GtkWidget *box;

    GtkWidget *optionBox;
    GtkWidget *logButton;
    GtkWidget *registButton;

    GtkWidget *infoBox;
    GtkWidget *usernameBox;
    GtkWidget *passwordBox;
    GtkWidget *usernameLable;
    GtkWidget *passwordLable;
    GtkWidget *sep;
    GtkWidget *image;


    //主窗口初始化
    info *userInfo;
    homepageWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_get_screen(homepageWindow);
    gtk_window_set_title(GTK_WINDOW(homepageWindow),"login in linpop");
    gtk_window_set_position(GTK_WINDOW(loginWindow),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(loginWindow),400,300);
    gtk_container_set_border_width(GTK_CONTAINER(loginWindow),20);

    //Big box：最外层box
    box = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(loginWindow),box);

    //原始icon图标
    iconImageRes = gdk_pixbuf_new_from_file_at_size("../res/icon.png",ICON_SIZE,ICON_SIZE,NULL);
    image = gtk_image_new_from_pixbuf(iconImageRes);
    gtk_box_pack_start(GTK_BOX(box),image,FALSE,FALSE,5);

    //input box
    infoBox = gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),infoBox,FALSE,FALSE,5);

    usernameBox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(infoBox),usernameBox,FALSE,FALSE,0);

    passwordBox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(infoBox),passwordBox,FALSE,FALSE,0);

    usernameLable = gtk_label_new("Login ID:");
    gtk_label_set_width_chars(usernameLable,15);
    gtk_box_pack_start(GTK_BOX(usernameBox),usernameLable,FALSE,FALSE,5);
    usernameText = gtk_entry_new();
    gtk_entry_set_max_length(usernameText,20);
    gtk_entry_set_text(GTK_ENTRY(usernameText),"12345");
    gtk_box_pack_start(GTK_BOX(usernameBox),usernameText,FALSE,FALSE,5);

    passwordLable = gtk_label_new("Password:");
    gtk_label_set_width_chars(passwordLable,15);
    gtk_box_pack_start(GTK_BOX(passwordBox),passwordLable,FALSE,FALSE,5);

    passwordText = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(passwordText),FALSE);
    gtk_entry_set_text(GTK_ENTRY(passwordText),"wqxpassword");
    gtk_entry_set_max_length(passwordText,20);
    gtk_box_pack_start(GTK_BOX(passwordBox),passwordText,FALSE,FALSE,5);

    sep = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(infoBox),sep,FALSE,FALSE,5);

    //button box
    optionBox = gtk_hbutton_box_new();
    gtk_box_pack_start(GTK_BOX(box),optionBox,FALSE,FALSE,5);

    registButton = gtk_button_new_with_label("Don't have an account?");
    g_signal_connect(G_OBJECT(registButton),"clicked",G_CALLBACK(on_button_clicked),(gpointer)REGIST);
    g_signal_connect_swapped(G_OBJECT(registButton),"clicked",G_CALLBACK(gtk_widget_hide),loginWindow);
    gtk_container_add(GTK_CONTAINER(optionBox),registButton);

    logButton = gtk_button_new_with_label("Log in");
    g_signal_connect(G_OBJECT(logButton),"clicked",G_CALLBACK(on_button_clicked),(gpointer)LOG_IN);
    g_signal_connect_swapped(G_OBJECT(logButton),"clicked",G_CALLBACK(gtk_widget_destroy),loginWindow);
    gtk_container_add(GTK_CONTAINER(optionBox),logButton);

    g_signal_connect(G_OBJECT(loginWindow),"delete_event",G_CALLBACK(delete_event), NULL);


    gtk_widget_show_all(loginWindow);

    gtk_main();



}