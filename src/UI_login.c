//
// Created by Anne Wu on 2019-08-30.
//
#include "../include/UI_login.h"

static GtkWidget *login_window;//登陆窗口
static GtkWidget *usernameText;//用户名
static GtkWidget *passwordText;//密码
static GtkWidget *passwordText2;//确认密码
static GtkWidget *dialog = NULL;//对话框
gboolean ishide = FALSE;//是否隐藏窗口

//void on_destroy(GtkWidget *widget,GdkEvent *event,gpointer* data){
//    sprintf(usernameText,"guest");
////    if(do_connect==TRUE){
////
////    }
//    gtk_widget_destroy(widget);
//}
void on_show(GtkButton* button,gpointer data){
    //如果未隐藏就恢复显示，由图标恢复为窗口
    if(ishide==FALSE){
        gtk_window_deiconify(GTK_WINDOW(login_window));
    }
    //如果隐藏就显示
    else{
        gtk_widget_show(login_window);
        ishide=FALSE;
    }
}
void on_hide(GtkButton* button,gpointer data) {
    //图标最小化
    gtk_window_iconify(GTK_WINDOW(login_window));
}
void on_login_delete(GtkWidget* window,gpointer data){
    //隐藏
    gtk_widget_hide(window);
    ishide=TRUE;

};
GtkWidget *create_view(gchar* filename){
    GtkWidget * window;
    GtkWidget* image;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),filename);
    gtk_container_set_border_width(GTK_CONTAINER(window),10);
    image = gtk_image_new_from_file(filename);
    gtk_container_add(GTK_CONTAINER(window),image);
    gtk_widget_show_all(window);
    return window;
}
void on_ok(GtkButton* button, gpointer data){
    const char* filename;
    GtkWidget* window;
    filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(data));
    window = create_view(filename);
    gtk_widget_show(window);
}
void on_cancel(GtkButton *button, gpointer data){
    gtk_widget_destroy(dialog);
}
void on_button_clicked (GtkWidget *button, gpointer data){
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(usernameText));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(passwordText));
    switch((int)data){
        case LOG_IN:
            //登陆
            g_print("username is:%s\n",username);
            g_print("password is:%s\n",password);
            homepageWindow();
            break;
        case REGIST:
            //注册
            registWindow();
            break;
        case REGIST_CANCEL:
            //取消注册
            loginWindow();
            break;
        case REGIST_CONFIRM:
            //确认注册
            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_OK_CANCEL,"Are you sure to regist?");
            gtk_widget_show(dialog);
            break;
        case ADD_IMAGE:
            //添加头像
            dialog = gtk_file_selection_new("choose a photo");
            g_signal_connect(G_OBJECT(dialog),"destroy",G_CALLBACK(gtk_widget_destroy),dialog);
            g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(dialog)->ok_button),"clicked",G_CALLBACK(on_ok),dialog);
            g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(dialog)->ok_button),"clicked",G_CALLBACK(on_cancel),NULL);
            g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(dialog)->cancel_button),"clicked",G_CALLBACK(on_cancel),NULL);
            gtk_widget_show(dialog);
            break;
    }
}

gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data){
    g_print("delete event occured\n");
    gtk_main_quit();
    return FALSE;
}

void registWindow(){
    GtkWidget *window;
    GtkWidget *box;

    GtkWidget *optionBox;
    GtkWidget *confirmButton;
    GtkWidget *cancelButton;

    GtkWidget *infoBox;
    GtkWidget *usernameBox;
    GtkWidget *passwordBox;
    GtkWidget *passwordBox2;
    GtkWidget *imageBox;
    GtkWidget *usernameLable;
    GtkWidget *passwordLable;
    GtkWidget *passwordLable2;
    GtkWidget *sep;
    GtkWidget *addImageButton;
    GtkWidget *image;

    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(delete_event),NULL);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_get_screen(window);
    gtk_window_set_title(GTK_WINDOW(window),"Register for linpop");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window),400,300);
    gtk_container_set_border_width(GTK_CONTAINER(window),20);

    //Big box
    box = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),box);

    image = gtk_image_new_from_file("../res/icon.png");
    gtk_box_pack_start(GTK_BOX(box),image,FALSE,FALSE,5);
    //input box
    infoBox = gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),infoBox,FALSE,FALSE,5);

    usernameBox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(infoBox),usernameBox,FALSE,FALSE,0);

    passwordBox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(infoBox),passwordBox,FALSE,FALSE,0);

    passwordBox2 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(infoBox),passwordBox2,FALSE,FALSE,0);

    imageBox = gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(infoBox),imageBox,FALSE,FALSE,0);

    usernameLable = gtk_label_new("Login ID:");
    gtk_label_set_width_chars(usernameLable,15);
    gtk_box_pack_start(GTK_BOX(usernameBox),usernameLable,FALSE,FALSE,5);
    usernameText = gtk_entry_new();
    gtk_entry_set_max_length(usernameText,20);
    gtk_entry_set_text(GTK_ENTRY(usernameText),"poplin");
    gtk_box_pack_start(GTK_BOX(usernameBox),usernameText,FALSE,FALSE,5);

    passwordLable = gtk_label_new("Password:");
    gtk_label_set_width_chars(passwordLable,15);
    gtk_box_pack_start(GTK_BOX(passwordBox),passwordLable,FALSE,FALSE,5);
    passwordText = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(passwordText),FALSE);
    gtk_entry_set_max_length(passwordText,20);
    gtk_box_pack_start(GTK_BOX(passwordBox),passwordText,FALSE,FALSE,5);

    passwordLable2 = gtk_label_new("Confirm Password:");
    gtk_label_set_width_chars(passwordLable2,15);
    gtk_box_pack_start(GTK_BOX(passwordBox2),passwordLable2,FALSE,FALSE,5);
    passwordText2 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(passwordText2),FALSE);
    gtk_entry_set_max_length(passwordText,20);
    gtk_box_pack_start(GTK_BOX(passwordBox2),passwordText2,FALSE,FALSE,5);



    addImageButton = gtk_button_new_with_label("choose photo from library");
    gtk_box_pack_start(GTK_BOX(imageBox),addImageButton,FALSE,FALSE,5);
    g_signal_connect(G_OBJECT(addImageButton),"clicked",G_CALLBACK(on_button_clicked),(gpointer)ADD_IMAGE);

    sep = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(infoBox),sep,FALSE,FALSE,5);

    //button box
    optionBox = gtk_hbutton_box_new();
    gtk_box_pack_start(GTK_BOX(box),optionBox,FALSE,FALSE,5);

    cancelButton = gtk_button_new_with_label("Cancel");
    /*
     * 点击按钮后REGIST窗口应当关闭，这里有bug
     * */

    g_signal_connect(G_OBJECT(cancelButton),"clicked",G_CALLBACK(on_button_clicked),(gpointer)REGIST_CANCEL);
    g_signal_connect_swapped(G_OBJECT(cancelButton),"clicked",G_CALLBACK(gtk_widget_destroy),window);
    gtk_container_add(GTK_CONTAINER(optionBox),cancelButton);

    confirmButton = gtk_button_new_with_label("Confirm");
    g_signal_connect(G_OBJECT(confirmButton),"clicked",G_CALLBACK(on_button_clicked),(gpointer)REGIST_CONFIRM);
    g_signal_connect_swapped(G_OBJECT(confirmButton),"clicked",G_CALLBACK(gtk_widget_destroy),window);

    gtk_container_add(GTK_CONTAINER(optionBox),confirmButton);

    gtk_widget_show_all(window);
}
void loginWindow(int argc, char *argv[]){
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

    gtk_init(&argc,&argv);
    g_signal_connect(G_OBJECT(login_window),"delete_event",G_CALLBACK(delete_event), NULL);
    login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_get_screen(login_window);
    gtk_window_set_title(GTK_WINDOW(login_window),"login in linpop");
    gtk_window_set_position(GTK_WINDOW(login_window),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(login_window),400,300);
    gtk_container_set_border_width(GTK_CONTAINER(login_window),20);

    //Big box
    box = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(login_window),box);

    image = gtk_image_new_from_file("../res/icon.png");
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
    gtk_entry_set_text(GTK_ENTRY(usernameText),"poplin");
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
    g_signal_connect_swapped(G_OBJECT(registButton),"clicked",G_CALLBACK(on_login_delete),login_window);
    gtk_container_add(GTK_CONTAINER(optionBox),registButton);

    logButton = gtk_button_new_with_label("Log in");
    g_signal_connect(G_OBJECT(logButton),"clicked",G_CALLBACK(on_button_clicked),(gpointer)LOG_IN);
    g_signal_connect_swapped(G_OBJECT(logButton),"clicked",G_CALLBACK(gtk_widget_destroy),login_window);
    gtk_container_add(GTK_CONTAINER(optionBox),logButton);

    gtk_widget_show_all(login_window);
    gtk_main();

}
