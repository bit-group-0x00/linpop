//
// Created by Anne Wu on 2019-08-30.
//
#include "../include/UI_login.h"
static GtkWidget *usernameText;
static GtkWidget *passwdText;
void on_button_clicked (GtkWidget *button, gpointer data){
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(usernameText));
    const gchar *passwd = gtk_entry_get_text(GTK_ENTRY(passwdText));
    g_print("Entry contents:%s\n",username);
    g_print("Entry contents:%s\n",passwd);
}
void loginWindow(int argc, char *argv[]){

//    GtkWidget *passwdText = gtk_entry_new();
//    passwdText = gtk_entry_new_with_max_length(20);
//    gtk_entry_set_text(GTK_ENTRY(passwdText),"wwwwwwwww");

    GtkWidget *window;
    GtkWidget *box;

    GtkWidget *optionBox;
    GtkWidget *cancelButton;
    GtkWidget *enterButton;
    GtkWidget *registButton;

    GtkWidget *infoBox;
    GtkWidget *usernameBox;
    GtkWidget *passwdBox;
    GtkWidget *usernameLable;
    GtkWidget *passwdLable;

    gtk_init(&argc,&argv);
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_get_screen(window);
    gtk_window_set_title(GTK_WINDOW(window),"login in linpop");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window),400,300);
    gtk_container_set_border_width(GTK_CONTAINER(window),20);

    //Big box
    box = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),box);

    //input box
    infoBox = gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),infoBox,FALSE,FALSE,5);

    usernameBox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(infoBox),usernameBox,FALSE,FALSE,0);

    passwdBox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(infoBox),passwdBox,FALSE,FALSE,0);

    usernameLable = gtk_label_new("Login ID:");
    gtk_label_set_width_chars(usernameLable,10);
    gtk_box_pack_start(GTK_BOX(usernameBox),usernameLable,FALSE,FALSE,5);

    usernameText = gtk_entry_new();
    gtk_entry_set_max_length(usernameText,20);
    gtk_entry_set_text(GTK_ENTRY(usernameText),"poplin");
    gtk_box_pack_start(GTK_BOX(usernameBox),usernameText,FALSE,FALSE,5);

    passwdLable = gtk_label_new("Password:");
    gtk_label_set_width_chars(passwdLable,10);
    gtk_box_pack_start(GTK_BOX(passwdBox),passwdLable,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(passwdBox),passwdText,FALSE,FALSE,5);

    passwdText = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(passwdText),FALSE);
    gtk_entry_set_text(GTK_ENTRY(passwdText),"poplin");
    gtk_entry_set_max_length(passwdText,20);
    gtk_box_pack_start(GTK_BOX(passwdBox),passwdText,FALSE,FALSE,5);

    //button box
    optionBox = gtk_hbutton_box_new();
    gtk_box_pack_start(GTK_BOX(box),optionBox,FALSE,FALSE,5);

    enterButton = gtk_button_new_with_label("Enter");
    g_signal_connect(G_OBJECT(enterButton),"clicked",G_CALLBACK(on_button_clicked),NULL);
    g_signal_connect(G_OBJECT(enterButton),"clicked",G_CALLBACK(gtk_widget_destroy),window);
    gtk_container_add(GTK_CONTAINER(optionBox),enterButton);


    cancelButton = gtk_button_new_with_label("Cancel");
//    g_signal_connect(G_OBJECT(enterButton),"clicked",G_CALLBACK(on_button_clicked),NULL);
    gtk_container_add(GTK_CONTAINER(optionBox),cancelButton);

    registButton = gtk_button_new_with_label("Don't have an account?");
//    g_signal_connect(G_OBJECT(enterButton),"clicked",G_CALLBACK(on_button_clicked),NULL);
    gtk_container_add(GTK_CONTAINER(optionBox),registButton);

    gtk_widget_show_all(window);
    gtk_main();

}