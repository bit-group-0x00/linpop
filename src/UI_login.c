//
// Created by Anne Wu on 2019-08-30.
//
#include "../include/UI_interface.h"
#include "../include/NET_client.h"


static GtkWidget *loginWindow;//登陆窗口
static GtkWidget *registWindow;//注册窗口
static GtkWidget *usernameText = NULL;//用户名
static GtkWidget *signatureText = NULL;//用户名
static GtkWidget *passwordText = NULL;//密码
static GtkWidget *passwordText2 = NULL;//确认密码


static GdkPixbuf *iconImageRes;
static GtkWidget *avaterImage;
static char *filename;
static char *signature;
static state userID;



GtkWidget *create_image(gchar *filename,gint size){
    GtkWidget *image;
    GdkPixbuf *res;
    res = gdk_pixbuf_new_from_file_at_size(filename,size,size,NULL);
    image = gtk_image_new_from_pixbuf(res);
    return image;
}
static gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    g_print("delete event occured\n");
    gtk_main_quit();
    return TRUE;
}

void show_error(GtkWidget *widget, gpointer window, gchar *message) {
    //错误信息提示对话框
    GtkWidget *dialog;
    GtkWidget *image = create_image("../res/icons8_error.png",36);
    GtkWidget *label = gtk_label_new(message);
    label_font(label,message,20,DARK_PURPLE,"low",DARK_PURPLE);
    GtkWidget *content_area;
    dialog = gtk_dialog_new_with_buttons("Question",NULL,GTK_DIALOG_DESTROY_WITH_PARENT,"OK",GTK_RESPONSE_YES,NULL);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_keep_above(GTK_WINDOW(dialog),TRUE);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_add(GTK_CONTAINER(content_area),image);
    gtk_container_add(GTK_CONTAINER(content_area),label);
    g_signal_connect_swapped(G_OBJECT(dialog),"delete_event",G_CALLBACK(delete_event),NULL);
    gtk_widget_show_all(GTK_WIDGET(dialog));
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

gint show_question(GtkWidget *widget, gpointer window, gchar *message) {
    //选择对话框，返回用户选项
    GtkWidget *dialog;
    GtkWidget *image = create_image("../res/icons_question.png",36);
    GtkWidget *label = gtk_label_new(message);
    label_font(label,message,16,DARK_PURPLE,"low",DARK_PURPLE);
    GtkWidget *content_area;
    dialog = gtk_dialog_new_with_buttons("Question",NULL,GTK_DIALOG_DESTROY_WITH_PARENT,"NO",GTK_RESPONSE_NO,"YES",GTK_RESPONSE_YES,NULL);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_keep_above(GTK_WINDOW(dialog),TRUE);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_add(GTK_CONTAINER(content_area),image);
    gtk_container_add(GTK_CONTAINER(content_area),label);
    gtk_widget_show_all(GTK_WIDGET(dialog));
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    g_signal_connect_swapped(G_OBJECT(dialog),"delete_event",G_CALLBACK(delete_event),NULL);
    gtk_widget_destroy(GTK_WIDGET(dialog));
    return result;
}

void show_info(GtkWidget *widget, gpointer window, gchar *message) {
    //提示信息对话框
    GtkWidget *dialog;
    GtkWidget *image = create_image("../res/icons_info.png",36);
    GtkWidget *label = gtk_label_new(message);
    label_font(label,message,20,DARK_PURPLE,"none",DARK_PURPLE);
    GtkWidget *content_area;
    dialog = gtk_dialog_new_with_buttons("Question",NULL,GTK_DIALOG_DESTROY_WITH_PARENT,"OK",GTK_RESPONSE_NONE,NULL);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_keep_above(GTK_WINDOW(dialog),TRUE);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    g_signal_connect_swapped(G_OBJECT(dialog),"delete_event",G_CALLBACK(delete_event),NULL);
    gtk_container_add(GTK_CONTAINER(content_area),image);
    gtk_container_add(GTK_CONTAINER(content_area),label);
    gtk_widget_show_all(GTK_WIDGET(dialog));
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(GTK_WIDGET(dialog));
}
void update_message(state type, void *newIncome){
    //提示信息对话框
    gchar* message = "";
    int newID = 0;
    gchar* name = "";
    if (type == SEND_MESSAGE || type == ADD_FRIEND_REQUEST){
        friend *fri = (friend *)newIncome;
        message = fri->last_msg->content;
        newID = fri->fri_pro.id;
        name = fri->fri_pro.nick_name;
    }
    else if ( type == SEND_MESSAGE_TO_GROUP || type == CREATE_GROUP_REQUEST){
        group *gro = (group *)newIncome;
        message = gro->last_msg->content;
        newID = gro->gro_pro.id;
        name = gro->gro_pro.name;
    }
    show_info(NULL,NULL,g_strdup_printf("你有一条来自%s的新消息：%s",name,message));
    switch (type){
        case SEND_MESSAGE:
            if(alreadyOpenFriendList[newID-10000]==TRUE){
                friend_msg_listener(message);
            }
            else{
                friend_chat_window(userID,newID);
            }
            break;
        case SEND_MESSAGE_TO_GROUP:
            if(alreadyOpenGroupList[newID]==TRUE){
                update_group((group *)newIncome);
            }
            else{
                group_chat_window(userID,newID);
            }
            group_chat_window(userID,newID);
            break;
        case ADD_FRIEND_REQUEST:
            show_info(NULL,NULL,g_strdup_printf("%d请求添加你的好友：%s",newID,name));
            break;
        case CREATE_GROUP_REQUEST:
            show_info(NULL,NULL,g_strdup_printf("%d请求把你加入群组：%s",newID,name));
            break;
        default:
            break;
    }
}

static void file_chooser_dialog() {
    GtkWidget *fileChooserDialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    fileChooserDialog = gtk_file_chooser_dialog_new("Choose a photo",
            GTK_WINDOW(registWindow),
            action,
            "CANCEL",
            GTK_RESPONSE_CANCEL,
            "OPEN",
            GTK_RESPONSE_ACCEPT,
            NULL);
    res = gtk_dialog_run(GTK_DIALOG(fileChooserDialog));
    if(res == GTK_RESPONSE_ACCEPT){
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(fileChooserDialog);
        filename = gtk_file_chooser_get_filename(chooser);
        iconImageRes = gdk_pixbuf_new_from_file_at_size(filename,ICON_SIZE,ICON_SIZE,NULL);
        gtk_image_set_from_pixbuf(GTK_IMAGE(avaterImage),iconImageRes);
    }
    gtk_widget_destroy(fileChooserDialog);

}

void on_button_clicked(GtkWidget *button, gpointer data) {
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(usernameText));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(passwordText));
    const gchar *password2 = gtk_entry_get_text(GTK_ENTRY(passwordText2));
    const gchar *signature = gtk_entry_get_text(GTK_ENTRY(signatureText));
    gchar *infoTitle = "Congratulation!\nYour userID is: ";
    gint result;
    switch ((int) data) {
        case LOG_IN:
            //登陆
//            g_print("username is:%s\n",username);
//            g_print("password is:%s\n",password);
            userID = strtol(username, NULL, 10);
            switch (login(userID, password)) {
                case SUCCESS:
                    gtk_widget_destroy(loginWindow);
                    homepage_window(userID);
                    break;
                case FAILURE:
                    show_error(NULL, NULL, "Your ID is not existing");
                    login_window();
                    break;
                case ERROR:
                    show_error(NULL, NULL, "Bad network, please try again");
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
            gtk_widget_hide(registWindow);
            login_window();
            break;
        case REGIST_CONFIRM:
            //确认注册
            result = show_question(loginWindow, NULL, "Is every information is right?");
//            g_print("密码比对：\nUSERNAME:%s\nPASSWORD:%s\n",password2,password);
            if (result == GTK_RESPONSE_YES) {
                //用户确定创建账户
                if (strcmp(password, password2) == 0 && g_utf8_strlen(password,-1)!=0) {
                    //两次输入的密码一样
                    userID = regist(username, password,signature,"");
                    switch (userID) {
                        case FAILURE:
                            g_print("Registered process failed：\nUSERNAME:%s\nPASSWORD:%s\n", username, password);
                            show_error(NULL, NULL, "Registered process failed");
                            regist_window();
                            break;
                        case ERROR:
                            g_print("Registered process Errored：\nUSERNAME:%s\nPASSWORD:%s\n", username, password);
                            show_error(NULL, NULL, "Registered process Errored");
                            regist_window();
                            break;
                        default:
                            g_print("创建成功：\nUSERNAME:%s\nPASSWORD:%s\n", username, password);
                            g_print("USERID: %d\nUSERINFO：\n%s\n", userID, infoTitle);
                            gchar *passwordFuck = "";
                            passwordFuck = g_strdup_printf("%s", password);
                            infoTitle = g_strdup_printf("%s%d", infoTitle, userID);
                            show_info(NULL, NULL, infoTitle);
                            gtk_widget_destroy(registWindow);
                            gtk_widget_destroy(loginWindow);
                            //头像传输bug
                            login(userID, passwordFuck);
                            homepage_window(userID);
                            break;
                    }
                }else if(g_utf8_strlen(password,-1)==0){
                    show_error(NULL,NULL,"密码不能为空");
                }else {
                    //两次输入的账号不一样
                    show_error(NULL, NULL, "两次输入的密码不同");
                    regist_window();
                    g_print("两次输入的密码不同！");
                }
                g_print("confirm\n");
            } else {
                g_print("cancel\n");
                regist_window();
            }

            break;
        case ADD_IMAGE:
            //添加头像
            file_chooser_dialog();
            break;
    }
}


void regist_window() {
    GtkWidget *box;

    GtkWidget *optionBox;
    GtkWidget *confirmButton;
    GtkWidget *cancelButton;

    GtkWidget *infoBox;
    GtkWidget *usernameBox;
    GtkWidget *passwordBox;
    GtkWidget *passwordBox2;
    GtkWidget *imageBox;
    GtkWidget *usernamelabel;
    GtkWidget *passwordlabel;
    GtkWidget *passwordlabel2;
    GtkWidget *sep;
    GtkWidget *addImageButton;

    registWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_get_screen(registWindow);
    gtk_window_set_title(GTK_WINDOW(registWindow), "Registered for linpop");
    gtk_window_set_position(GTK_WINDOW(registWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(registWindow), 400, 300);
    gtk_container_set_border_width(GTK_CONTAINER(registWindow), 20);
    gtk_window_set_opacity(GTK_WINDOW(registWindow),0.7);
    //Big box
    box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(registWindow), box);


    avaterImage = gtk_image_new_from_pixbuf(iconImageRes);
    gtk_box_pack_start(GTK_BOX(box), avaterImage, FALSE, FALSE, 5);

    //input box
    infoBox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), infoBox, FALSE, FALSE, 5);

    usernameBox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(infoBox), usernameBox, FALSE, FALSE, 0);

    GtkWidget *signatureBox;
    signatureBox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(infoBox), signatureBox, FALSE, FALSE, 0);

    passwordBox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(infoBox), passwordBox, FALSE, FALSE, 0);

    passwordBox2 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(infoBox), passwordBox2, FALSE, FALSE, 0);

    imageBox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(infoBox), imageBox, FALSE, FALSE, 0);

    usernamelabel = gtk_label_new("Your nickname:");
    gtk_label_set_width_chars(usernamelabel, 15);
    gtk_box_pack_start(GTK_BOX(usernameBox), usernamelabel, FALSE, FALSE, 5);
    usernameText = gtk_entry_new();
    gtk_entry_set_max_length(usernameText, 20);
    gtk_entry_set_text(GTK_ENTRY(usernameText), "poplin");
    gtk_box_pack_start(GTK_BOX(usernameBox), usernameText, FALSE, FALSE, 5);

    GtkWidget *signatureLable;
    signatureLable = gtk_label_new("用一句话介绍你吧:");
    gtk_label_set_width_chars(signatureLable, 15);
    gtk_box_pack_start(GTK_BOX(signatureBox), signatureLable, FALSE, FALSE, 5);
    signatureText = gtk_entry_new();
    gtk_entry_set_max_length(signatureText, 20);
    gtk_entry_set_text(GTK_ENTRY(signatureText), "Hello world");
    gtk_box_pack_start(GTK_BOX(signatureBox), signatureText, FALSE, FALSE, 5);

    passwordlabel = gtk_label_new("Your password:");
    gtk_label_set_width_chars(passwordlabel, 15);
    gtk_box_pack_start(GTK_BOX(passwordBox), passwordlabel, FALSE, FALSE, 5);

    passwordText = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(passwordText), FALSE);
    gtk_entry_set_max_length(passwordText, 20);
    gtk_box_pack_start(GTK_BOX(passwordBox), passwordText, FALSE, FALSE, 5);

    passwordlabel2 = gtk_label_new("Confirm Password:");
    gtk_label_set_width_chars(passwordlabel2, 15);
    gtk_box_pack_start(GTK_BOX(passwordBox2), passwordlabel2, FALSE, FALSE, 5);

    passwordText2 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(passwordText2), FALSE);
    gtk_entry_set_max_length(passwordText2, 20);
    gtk_box_pack_start(GTK_BOX(passwordBox2), passwordText2, FALSE, FALSE, 5);


    addImageButton = gtk_button_new_with_label("choose photo from library");
    gtk_box_pack_start(GTK_BOX(imageBox), addImageButton, FALSE, FALSE, 5);
    g_signal_connect(G_OBJECT(addImageButton), "clicked", G_CALLBACK(on_button_clicked), (gpointer) ADD_IMAGE);

    sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(infoBox), sep, FALSE, FALSE, 5);

    //button box
    optionBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(box), optionBox, FALSE, FALSE, 5);

    cancelButton = gtk_button_new_with_label("Cancel");

    g_signal_connect(G_OBJECT(cancelButton), "clicked", G_CALLBACK(on_button_clicked), (gpointer) REGIST_CANCEL);
    g_signal_connect_swapped(G_OBJECT(cancelButton), "clicked", G_CALLBACK(gtk_widget_hide), registWindow);
    gtk_container_add(GTK_CONTAINER(optionBox), cancelButton);

    confirmButton = gtk_button_new_with_label("Confirm");
    g_signal_connect(G_OBJECT(confirmButton), "clicked", G_CALLBACK(on_button_clicked), (gpointer) REGIST_CONFIRM);
//    g_signal_connect_swapped(G_OBJECT(confirmButton), "clicked", G_CALLBACK(gtk_widget_destroy), registWindow);

    g_signal_connect(G_OBJECT(registWindow), "delete_event", G_CALLBACK(delete_event), NULL);

    gtk_container_add(GTK_CONTAINER(optionBox), confirmButton);

    gtk_widget_show_all(registWindow);
}

void login_window(int argc, char *argv[]) {
    GtkWidget *box;

    GtkWidget *optionBox;
    GtkWidget *logButton;
    GtkWidget *registButton;

    GtkWidget *infoBox;
    GtkWidget *usernameBox;
    GtkWidget *passwordBox;
    GtkWidget *usernamelabel;
    GtkWidget *passwordlabel;
    GtkWidget *sep;

    //主窗口初始化
    gtk_init(&argc, &argv);
    loginWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_get_screen(loginWindow);
    gtk_window_set_title(GTK_WINDOW(loginWindow), "login in linpop");
    gtk_window_set_position(GTK_WINDOW(loginWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(loginWindow), 400, 300);
    gtk_container_set_border_width(GTK_CONTAINER(loginWindow), 20);
    gtk_widget_set_opacity(GTK_WIDGET(loginWindow), 0.7);

    //Big box：最外层box
    box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(loginWindow), box);

    //原始icon图标
    iconImageRes = gdk_pixbuf_new_from_file_at_size("../res/icon.png", ICON_SIZE, ICON_SIZE, NULL);
    avaterImage = gtk_image_new_from_pixbuf(iconImageRes);
    gtk_box_pack_start(GTK_BOX(box), avaterImage, FALSE, FALSE, 5);

    //input box
    infoBox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), infoBox, FALSE, FALSE, 5);

    usernameBox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(infoBox), usernameBox, FALSE, FALSE, 0);

    passwordBox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(infoBox), passwordBox, FALSE, FALSE, 0);

    usernamelabel = gtk_label_new("Login ID:");
    gtk_label_set_markup(GTK_LABEL(usernamelabel),
                         g_strdup_printf("%s%s%s", "<span underline='none' underline_color='blue' font_desc='16'>",
                                         "LoginID", "</span>"));

    gtk_label_set_width_chars(usernamelabel, 15);
    gtk_box_pack_start(GTK_BOX(usernameBox), usernamelabel, FALSE, FALSE, 5);
    usernameText = gtk_entry_new();
    gtk_entry_set_max_length(usernameText, 20);
    gtk_entry_set_text(GTK_ENTRY(usernameText), "10223");
    gtk_box_pack_start(GTK_BOX(usernameBox), usernameText, FALSE, FALSE, 5);

    passwordlabel = gtk_label_new("Password:");
    label_font(passwordlabel, "Password", FONT_SIZE_SMALL, "black", "none", "blue");
    gtk_label_set_width_chars(passwordlabel, 15);
    gtk_box_pack_start(GTK_BOX(passwordBox), passwordlabel, FALSE, FALSE, 5);

    passwordText = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(passwordText), FALSE);
    gtk_entry_set_text(GTK_ENTRY(passwordText), "123456");
    gtk_entry_set_max_length(passwordText, 20);
    gtk_box_pack_start(GTK_BOX(passwordBox), passwordText, FALSE, FALSE, 5);

    sep = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(infoBox), sep, FALSE, FALSE, 5);

    //button box
    optionBox = gtk_hbutton_box_new();
    gtk_box_pack_start(GTK_BOX(box), optionBox, FALSE, FALSE, 5);

    registButton = gtk_button_new_with_label("Don't have an account?");
    g_signal_connect(G_OBJECT(registButton), "clicked", G_CALLBACK(on_button_clicked), (gpointer) REGIST);
    g_signal_connect_swapped(G_OBJECT(registButton), "clicked", G_CALLBACK(gtk_widget_hide), loginWindow);
    gtk_container_add(GTK_CONTAINER(optionBox), registButton);

    logButton = gtk_button_new_with_label("Log in");
    g_signal_connect(G_OBJECT(logButton), "clicked", G_CALLBACK(on_button_clicked), (gpointer) LOG_IN);
//    g_signal_connect_swapped(G_OBJECT(logButton), "clicked", G_CALLBACK(gtk_widget_destroy), loginWindow);
    gtk_container_add(GTK_CONTAINER(optionBox), logButton);

    g_signal_connect(G_OBJECT(loginWindow), "delete_event", G_CALLBACK(delete_event), NULL);


    gtk_widget_show_all(loginWindow);

    gtk_main();

}
