//
// Created by Anne Wu on 2019-08-30.
//
#include "../include/UI_login.h"

static GtkWidget *login_window;//登陆窗口
static GtkWidget *register_window;//注册窗口
static GtkWidget *usernameText;//用户名
static GtkWidget *passwordText;//密码
static GtkWidget *passwordText2;//确认密码
static GtkWidget *fileSelector = NULL;

GtkWidget *create_fileSelection_window(gchar* filename){
    GtkWidget * window;
    GtkWidget* image;
    g_print("Fileselector\n");
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),filename);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window),10);
    image = gtk_image_new_from_file(filename);
    gtk_container_add(GTK_CONTAINER(window),image);
    gtk_widget_show_all(window);
    return window;
}

void on_fileSelection_ok(GtkButton* button, gpointer data){
    const char* filename;
    GtkWidget* window;
    filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(data));
    window = create_fileSelection_window(filename);
    gtk_widget_show(window);
}

void on_button_clicked (GtkWidget *button, gpointer data){
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(usernameText));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(passwordText));
    const gchar *password2 = gtk_entry_get_text(GTK_ENTRY(passwordText2));
    gint result;
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
            gtk_widget_show(login_window);
            break;
        case REGIST_CONFIRM:
            //确认注册
            result = show_question(NULL,NULL,"Are you sure to register?");
            g_print("%d\n",result);
            g_print("密码比对：\nUSERNAME:%s\nPASSWORD:%s\n",password2,password);
            if(result==GTK_RESPONSE_YES){
                //用户确定创建账户
                if(strcmp(password,password2)==0){
                    //两次输入的账号一样
                    g_print("创建成功：\nUSERNAME:%s\nPASSWORD:%s\n",username,password);
                    show_info(NULL, NULL ,"Congratulation!");
                }
                else{
                    show_error(NULL,NULL,"passwords conflict");
                    g_print("passwords conflict");
                }
                g_print("confirm\n");
            }
            else{
                g_print("cancel\n");
            }

            break;
        case ADD_IMAGE:
            //添加头像
            fileSelector = gtk_file_selection_new("choose a photo");
            g_print("Fileselector\n");
            g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(fileSelector)->ok_button),"clicked",G_CALLBACK(on_fileSelection_ok),fileSelector);
            g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(fileSelector)->ok_button),"clicked",G_CALLBACK(gtk_widget_destroy),fileSelector);
            g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(fileSelector)->cancel_button),"clicked",G_CALLBACK(gtk_widget_destroy),NULL);
            g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(fileSelector)->cancel_button),"clicked",G_CALLBACK(gtk_widget_destroy),fileSelector);
            g_signal_connect(G_OBJECT(fileSelector),"destroy",G_CALLBACK(gtk_widget_destroy),NULL);
            gtk_widget_show(fileSelector);
            break;
    }
}

static gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data){
    g_print("delete event occured\n");
    gtk_main_quit();
    return FALSE;
}

void registWindow(){
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
    register_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_get_screen(register_window);
    gtk_window_set_title(GTK_WINDOW(register_window),"Register for linpop");
    gtk_window_set_position(GTK_WINDOW(register_window),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(register_window),400,300);
    gtk_container_set_border_width(GTK_CONTAINER(register_window),20);

    //Big box
    box = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(register_window),box);

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
    g_signal_connect_swapped(G_OBJECT(cancelButton),"clicked",G_CALLBACK(gtk_widget_destroy),register_window);
    gtk_container_add(GTK_CONTAINER(optionBox),cancelButton);

    confirmButton = gtk_button_new_with_label("Confirm");
    g_signal_connect(G_OBJECT(confirmButton),"clicked",G_CALLBACK(on_button_clicked),(gpointer)REGIST_CONFIRM);
    g_signal_connect_swapped(G_OBJECT(confirmButton),"clicked",G_CALLBACK(gtk_widget_destroy),register_window);

    g_signal_connect(G_OBJECT(login_window),"delete_event",G_CALLBACK(delete_event), NULL);

    gtk_container_add(GTK_CONTAINER(optionBox),confirmButton);

    gtk_widget_show_all(register_window);
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
    g_signal_connect_swapped(G_OBJECT(registButton),"clicked",G_CALLBACK(gtk_widget_hide),login_window);
    gtk_container_add(GTK_CONTAINER(optionBox),registButton);

    logButton = gtk_button_new_with_label("Log in");
    g_signal_connect(G_OBJECT(logButton),"clicked",G_CALLBACK(on_button_clicked),(gpointer)LOG_IN);
    g_signal_connect_swapped(G_OBJECT(logButton),"clicked",G_CALLBACK(gtk_widget_destroy),login_window);
    gtk_container_add(GTK_CONTAINER(optionBox),logButton);

    g_signal_connect(G_OBJECT(login_window),"delete_event",G_CALLBACK(delete_event), NULL);


    gtk_widget_show_all(login_window);
    gtk_main();

}
