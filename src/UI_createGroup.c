//
// Created by Anne Wu on 2019-08-30.
//
#include "../include/UI_interface.h"
#include "../include/NET_client.h"

static GtkWidget *createWindow;
static GtkWidget *groupNameText = NULL;//
static GtkWidget *groupInfoText = NULL;//
static GtkWidget *groupMemberText = NULL;//

static GdkPixbuf *iconImageRes;
static GtkWidget *avaterImage;
static gchar* avaterFilename;
static int memberNum;
static int memberId[100];

static gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_main_quit();
    gtk_widget_destroy(createWindow);
    return FALSE;
}

void read_friend_list(){
    const gchar *frindList = gtk_entry_get_text(GTK_ENTRY(groupMemberText));
    char p[10];
    int memnum = 0;
    memset(p,0, sizeof(p));
    p[0] = frindList[0];
    g_print("LENGTH:%d\n",g_utf8_strlen(frindList,-1));
    if (g_utf8_strlen(frindList,-1)==0){
        return;
    }
    memberId[memnum++] = my_info.my_pro.id;
    for (int i = 1; i < g_utf8_strlen(frindList,-1); ++i) {
        if(frindList[i]==';'){
            g_print("P:%s\n",p);
            memberId[memnum++] = strtol(p,NULL,10);
            g_print("memberId:%d\n",memberId[memnum-1]);
            memset(p, 0, sizeof(p));
            p[0] = frindList[i+1];
            i++;
        }
        else{
            p[strlen(p)] = frindList[i];
        }
    }
    memberId[memnum++] = strtol(p,NULL,10);
    memberNum = memnum;
}

static void file_chooser_dialog(GtkWidget *createWindow) {
    GtkWidget *fileChooserDialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    fileChooserDialog = gtk_file_chooser_dialog_new("Choose a photo",
                                                    GTK_WINDOW(createWindow),
                                                    action,
                                                    "CANCEL",
                                                    GTK_RESPONSE_CANCEL,
                                                    "OPEN",
                                                    GTK_RESPONSE_ACCEPT,
                                                    NULL);
    res = gtk_dialog_run(GTK_DIALOG(fileChooserDialog));
    if(res == GTK_RESPONSE_ACCEPT){
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(fileChooserDialog);
        avaterFilename = gtk_file_chooser_get_filename(chooser);
        g_print("%s",avaterFilename);
        iconImageRes = gdk_pixbuf_new_from_file_at_size(avaterFilename,ICON_SIZE,ICON_SIZE,NULL);
        gtk_image_set_from_pixbuf(GTK_IMAGE(avaterImage),iconImageRes);
    }
    gtk_widget_destroy(fileChooserDialog);

}

static void on_confirm_button_clicked(GtkWidget *button, gpointer data) {
    const gchar *groupName = gtk_entry_get_text(GTK_ENTRY(groupNameText));
    const gchar *groupInfo = gtk_entry_get_text(GTK_ENTRY(groupInfoText));
    read_friend_list();
    gchar *infoTitle = "Congratulation!\nYour userID is: ";
    gint result;
    g_print("CONFIRM_BUTTON_CLICKED");
    create_group(groupName,groupInfo,avaterFilename,memberNum,memberId);
    homepage_add_group(mainListbox);
    gtk_widget_destroy(createWindow);
}

void create_group_window() {
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
    GtkWidget *groupInfoLabel;
    GtkWidget *groupMemberIdLabel;
    GtkWidget *sep;
    GtkWidget *addImageButton;

    createWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_get_screen(createWindow);
    gtk_window_set_title(GTK_WINDOW(createWindow), "Registered for linpop");
    gtk_window_set_position(GTK_WINDOW(createWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(createWindow), 400, 300);
    gtk_container_set_border_width(GTK_CONTAINER(createWindow), 20);

    //Big box
    box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(createWindow), box);

    iconImageRes = gdk_pixbuf_new_from_file_at_size("../res/icons8_people.png",ICON_SIZE,ICON_SIZE,NULL);
    avaterImage = gtk_image_new_from_pixbuf(iconImageRes);
    gtk_box_pack_start(GTK_BOX(box), avaterImage, FALSE, FALSE, 5);

    //input box
    infoBox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), infoBox, FALSE, FALSE, 5);

    usernameBox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(infoBox), usernameBox, FALSE, FALSE, 0);

    passwordBox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(infoBox), passwordBox, FALSE, FALSE, 0);

    passwordBox2 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(infoBox), passwordBox2, FALSE, FALSE, 0);

    imageBox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(infoBox), imageBox, FALSE, FALSE, 0);

    usernamelabel = gtk_label_new("Group name:");
    gtk_label_set_width_chars(usernamelabel, 20);
    gtk_box_pack_start(GTK_BOX(usernameBox), usernamelabel, FALSE, FALSE, 5);
    groupNameText = gtk_entry_new();
    gtk_entry_set_max_length(groupNameText, 20);
    gtk_entry_set_text(GTK_ENTRY(groupNameText), "we are family");
    gtk_box_pack_start(GTK_BOX(usernameBox), groupNameText, FALSE, FALSE, 5);

    groupInfoLabel = gtk_label_new("Group information:");
    gtk_label_set_width_chars(groupInfoLabel, 20);
    gtk_box_pack_start(GTK_BOX(passwordBox), groupInfoLabel, FALSE, FALSE, 5);

    groupInfoText = gtk_entry_new();
    gtk_entry_set_max_length(groupInfoText, 20);
    gtk_box_pack_start(GTK_BOX(passwordBox), groupInfoText, FALSE, FALSE, 5);

    groupMemberIdLabel = gtk_label_new("群成员ID 以分号隔开:");
    gtk_label_set_width_chars(groupMemberIdLabel, 20);
    gtk_box_pack_start(GTK_BOX(passwordBox2), groupMemberIdLabel, FALSE, FALSE, 5);

    groupMemberText = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(passwordBox2), groupMemberText, FALSE, FALSE, 5);


    addImageButton = gtk_button_new_with_label("选择群头像");
    gtk_box_pack_start(GTK_BOX(imageBox), addImageButton, FALSE, FALSE, 5);
    g_signal_connect(G_OBJECT(addImageButton), "clicked", G_CALLBACK(file_chooser_dialog), (gpointer) ADD_IMAGE);

    sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(infoBox), sep, FALSE, FALSE, 5);

    //button box
    optionBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(box), optionBox, FALSE, FALSE, 5);

    cancelButton = gtk_button_new_with_label("Cancel");

    g_signal_connect(G_OBJECT(cancelButton), "clicked", G_CALLBACK(delete_event), NULL);
    g_signal_connect_swapped(G_OBJECT(cancelButton), "clicked", G_CALLBACK(gtk_widget_destroy), NULL);
    gtk_container_add(GTK_CONTAINER(optionBox), cancelButton);

    confirmButton = gtk_button_new_with_label("Confirm");
    g_signal_connect(G_OBJECT(confirmButton), "clicked", G_CALLBACK(on_confirm_button_clicked), NULL);
    g_signal_connect_swapped(G_OBJECT(confirmButton), "clicked", G_CALLBACK(delete_event), NULL);

    gtk_container_add(GTK_CONTAINER(optionBox), confirmButton);

    gtk_widget_show_all(createWindow);

    g_signal_connect(G_OBJECT(createWindow), "delete_event", G_CALLBACK(delete_event), NULL);


}