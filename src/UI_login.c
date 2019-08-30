//
// Created by Anne Wu on 2019-08-30.
//
#include "../include/UI_login.h"
void loginWindow(int argc, char *argv[]){
    GtkWidget *window;
    GtkWidget *cancelButton;
    GtkWidget *enterButton;
    GtkWidget *registButton;
    GtkWidget *usernameLable;
    GtkWidget *passwdLable;
/*  helloGtk测试文件
    gtk_init(&argc,&argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_show(window);
    gtk_main();
    return;
//*/
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
    gtk_window_set_title(GTK_WINDOW(window),"login in linpop");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window),200,150);


}