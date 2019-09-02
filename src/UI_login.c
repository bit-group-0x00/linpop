//
// Created by Anne Wu on 2019-08-30.
//
#include "../include/UI_interface.h"
#include "../include/NET_client.h"

#define ICON_SIZE 80

static GtkWidget *loginWindow;//登陆窗口
static GtkWidget *registWindow;//注册窗口
static GtkWidget *usernameText = NULL;//用户名
static GtkWidget *passwordText = NULL;//密码
static GtkWidget *passwordText2 = NULL;//确认密码
static GtkWidget *fileSelector = NULL;

static GdkPixbuf *iconImageRes;
static GdkImage *image;
static state userID;

void show_error(GtkWidget *widget, gpointer window, gchar* message)
{
    //错误信息提示对话框
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(NULL,
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    message);
    gtk_window_set_title(GTK_WINDOW(dialog), "Error");
    gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

gint show_question(GtkWidget *widget, gpointer window, gchar* message)
{
    //选择对话框，返回用户选项
    GtkWidget *dialog;
    gint result;
    dialog = gtk_message_dialog_new(NULL,
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION,
                                    GTK_BUTTONS_YES_NO,
                                    message);
    gtk_window_set_title(GTK_WINDOW(dialog), "Question");
    gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
    result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return result;
}

void show_info(GtkWidget *widget, gpointer window, gchar* message)
{
    //提示信息对话框
    GtkWidget *dialog;
    g_print("show_info:%s\n",message);
    dialog = gtk_message_dialog_new(NULL,
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_INFO,
                                    GTK_BUTTONS_OK,
                                    message, "title");
    gtk_window_set_title(GTK_WINDOW(dialog), "Information");
    gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_fileSelection_ok(GtkButton* button, gpointer data){
    //文件选择确定
    const char* filename;
    filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(data));
    iconImageRes = gdk_pixbuf_new_from_file_at_size(filename,ICON_SIZE,ICON_SIZE,NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image),iconImageRes);
}
void on_fileSelection_cancel(GtkButton* button, gpointer data){
    //文件选择取消
    gtk_widget_destroy(fileSelector);
}

void on_button_clicked (GtkWidget *button, gpointer data){
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(usernameText));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(passwordText));
    const gchar *password2 = gtk_entry_get_text(GTK_ENTRY(passwordText2));
    gchar *infoTitle = "Congratulation!\nYour userID is: ";
    gint result;
    switch((int)data){
        case LOG_IN:
            //登陆
//            g_print("username is:%s\n",username);
//            g_print("password is:%s\n",password);
            userID = strtol(username,NULL,10);
            switch(login(userID,password)){
                case SUCCESS:
                    homepage_window(userID);
                    break;
                case FAILURE:
                    show_error(NULL,NULL,"Your ID is not existing");
                    login_window();
                    break;
                case ERROR:
                    show_error(NULL,NULL,"Bad network, please try again");
                    login_window();
                    break;
            }
            break;
        case REGIST:
            //注册
            regist_window();
            break;
        case REGIST_CANCEL:
            //取消注册
            gtk_widget_destroy(registWindow);
            login_window();
            break;
        case REGIST_CONFIRM:
            //确认注册
            result = show_question(NULL,NULL,"Is every information is right?");
//            g_print("密码比对：\nUSERNAME:%s\nPASSWORD:%s\n",password2,password);
            if(result==GTK_RESPONSE_YES){
                //用户确定创建账户
                if(strcmp(password,password2)==0){
                    //两次输入的账号一样
                    userID = regist(username,password);
                    switch(userID){
                        case FAILURE:
                            g_print("Registered process failed：\nUSERNAME:%s\nPASSWORD:%s\n",username,password);
                            show_error(NULL,NULL,"Registered process failed");
                            regist_window();
                            break;
                        case ERROR:
                            g_print("Registered process Errored：\nUSERNAME:%s\nPASSWORD:%s\n",username,password);
                            show_error(NULL,NULL,"Registered process Errored");
                            regist_window();
                            break;
                        default:
                            g_print("创建成功：\nUSERNAME:%s\nPASSWORD:%s\n",username,password);
                            g_print("USERID: %d\nUSERINFO：\n%s\n",userID,infoTitle);
                            infoTitle = g_strdup_printf("%s%d",infoTitle,userID);
                            show_info(NULL, NULL ,infoTitle);
                            gtk_widget_destroy(registWindow);
                            login(userID,password);
                            homepage_window(userID);
                            break;
                    }
                }
                else{
                    //两次输入的账号不一样
                    show_error(NULL,NULL,"passwords conflict");
                    regist_window();
                    g_print("passwords conflict");
                }
                g_print("confirm\n");
            }
            else{
                g_print("cancel\n");
                regist_window();
            }

            break;
        case ADD_IMAGE:
            //添加头像
            fileSelector = gtk_file_selection_new("choose a photo");
            g_print("Fileselector\n");
            g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(fileSelector)->ok_button),"clicked",G_CALLBACK(on_fileSelection_ok),fileSelector);
            g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(fileSelector)->ok_button),"clicked",G_CALLBACK(on_fileSelection_cancel),NULL);
            g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(fileSelector)->cancel_button),"clicked",G_CALLBACK(on_fileSelection_cancel),NULL);

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

void regist_window(int argc, char *argv[]){
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

    registWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_get_screen(registWindow);
    gtk_window_set_title(GTK_WINDOW(registWindow),"Registered for linpop");
    gtk_window_set_position(GTK_WINDOW(registWindow),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(registWindow),400,300);
    gtk_container_set_border_width(GTK_CONTAINER(registWindow),20);

    //Big box
    box = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(registWindow),box);


    image = gtk_image_new_from_pixbuf(iconImageRes);
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

    usernameLable = gtk_label_new("Your nickname:");
    gtk_label_set_width_chars(usernameLable,15);
    gtk_box_pack_start(GTK_BOX(usernameBox),usernameLable,FALSE,FALSE,5);
    usernameText = gtk_entry_new();
    gtk_entry_set_max_length(usernameText,20);
    gtk_entry_set_text(GTK_ENTRY(usernameText),"poplin");
    gtk_box_pack_start(GTK_BOX(usernameBox),usernameText,FALSE,FALSE,5);

    passwordLable = gtk_label_new("Your password:");
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
    gtk_entry_set_max_length(passwordText2,20);
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

    g_signal_connect(G_OBJECT(cancelButton),"clicked",G_CALLBACK(on_button_clicked),(gpointer)REGIST_CANCEL);
    g_signal_connect_swapped(G_OBJECT(cancelButton),"clicked",G_CALLBACK(gtk_widget_destroy),registWindow);
    gtk_container_add(GTK_CONTAINER(optionBox),cancelButton);

    confirmButton = gtk_button_new_with_label("Confirm");
    g_signal_connect(G_OBJECT(confirmButton),"clicked",G_CALLBACK(on_button_clicked),(gpointer)REGIST_CONFIRM);
    g_signal_connect_swapped(G_OBJECT(confirmButton),"clicked",G_CALLBACK(gtk_widget_destroy),registWindow);

    g_signal_connect(G_OBJECT(registWindow),"delete_event",G_CALLBACK(delete_event), NULL);

    gtk_container_add(GTK_CONTAINER(optionBox),confirmButton);

    gtk_widget_show_all(registWindow);
}
void login_window(int argc, char *argv[]){
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
    gtk_init(&argc,&argv);
    loginWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_get_screen(loginWindow);
    gtk_window_set_title(GTK_WINDOW(loginWindow),"login in linpop");
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
//    gtk_label_set_markup(GTK_LABEL(usernameLable),g_strdup_printf("%s%s%s","<span foreground='red' underline='double' underline_color='blue' font_desc='32'>","LoginID","</span>"));

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
