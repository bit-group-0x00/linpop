//
// Created by yuanyuanyaun on 9/2/19.
//
#include "../include/UI_interface.h"
#include "../include/NET_client.h"

static int userId;
static int groupId;
static gchar* filename;
static GtkWidget* listBox;




GtkToolItem* new_tool_item(gchar *icon,gchar *label)
{
    GdkPixbuf *src = gdk_pixbuf_new_from_file(icon, NULL);;
    GdkPixbuf *dst = gdk_pixbuf_scale_simple(src, 32, 32, GDK_INTERP_BILINEAR);
    GtkWidget *iconFile = gtk_image_new_from_pixbuf(dst);
    g_object_unref(src);
    g_object_unref(dst);
    GtkToolItem *item = gtk_tool_button_new(iconFile,label);
    return item;
}
static void file_button_callback() {
    GtkWidget *fileChooserDialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    fileChooserDialog = gtk_file_chooser_dialog_new("Choose a photo",
                                                    NULL,
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
    }
    gtk_widget_destroy(fileChooserDialog);

}
void icon_button_callback(){
    g_print("1\n");
}
void history_button_callback(){
    g_print("3\n");
}
void exit_button_callback(){
    g_print("4\n");
}
void invite_button_callback(){
    g_print("5\n");
}
GtkWidget *new_Box(gchar*messageSenderIcon,int senderId,gchar*messageSenderName,gchar*message){
    GtkWidget *messageBox;
    GtkWidget *senderIcon;
    GdkPixbuf *src;
    GdkPixbuf *dst;

    GtkWidget *text;
    GtkTextBuffer *textBuffer;

    messageBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    src = gdk_pixbuf_new_from_file(messageSenderIcon, NULL);
    dst = gdk_pixbuf_scale_simple(src, 20, 20, GDK_INTERP_BILINEAR);
    senderIcon = gtk_image_new_from_pixbuf(dst);
    g_object_unref(src);
    g_object_unref(dst);

    GtkWidget *imageBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    gtk_box_pack_start(GTK_BOX(imageBox),senderIcon,FALSE,FALSE,0);

    // alignmentImg = gtk_alignment_new(0,0,0,0);
    gtk_box_pack_start(GTK_BOX(messageBox),imageBox ,FALSE,FALSE,5);

    text = gtk_text_view_new();
    textBuffer = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(textBuffer,message,-1);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(text),textBuffer);

    gtk_box_pack_start(GTK_BOX(messageBox),text,FALSE,FALSE,5);
    g_print("%s",message);
    return  messageBox;
}
void insert_box(GtkWidget *message){
    gtk_list_box_insert(GTK_LIST_BOX(listBox),message,-1);
    g_print("INSERT");
}
void send_button_callback(GtkWidget *button,gpointer buffer){
    gchar *message;
    GtkTextIter start,end;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer),&start,&end);
    message = gtk_text_buffer_get_text(buffer,&start,&end,FALSE);
    g_print("%s",message);
    GtkWidget* messageBox = new_Box("../res/icon.png",1223333,"yuanyuanyuan",message);
    insert_box(messageBox);
    gtk_text_buffer_delete(buffer,&start,&end);
}

//void add_item_list_box(gchar*messageSenderIcon,int senderId,gchar*messageSenderName,gchar*message){
//    GtkWidget *messageBox;
//
//    GtkWidget *senderIcon;
//    GdkPixbuf *src;
//    GdkPixbuf *dst;
//
//    GtkWidget *text;
//    GtkTextBuffer *textBuffer;
//
//    messageBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
//    src = gdk_pixbuf_new_from_file(messageSenderIcon, NULL);
//    dst = gdk_pixbuf_scale_simple(src, 20, 20, GDK_INTERP_BILINEAR);
//    senderIcon = gtk_image_new_from_pixbuf(dst);
//    g_object_unref(src);
//    g_object_unref(dst);
//
//    GtkWidget *imageBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
//    gtk_box_pack_start(GTK_BOX(imageBox),senderIcon,FALSE,FALSE,0);
//
//   // alignmentImg = gtk_alignment_new(0,0,0,0);
//    gtk_box_pack_start(GTK_BOX(messageBox),imageBox ,FALSE,FALSE,5);
//
//    text = gtk_text_view_new();
//    textBuffer = gtk_text_buffer_new(NULL);
//    gtk_text_buffer_set_text(textBuffer,message,-1);
//    gtk_text_view_set_buffer(GTK_TEXT_VIEW(text),textBuffer);
//
//    gtk_box_pack_start(GTK_BOX(messageBox),text,FALSE,FALSE,5);
//
//    gtk_list_box_insert(GTK_LIST_BOX(listBox),messageBox,-1);
//    gtk_widget_show(GTK_WIDGET(messageBox));
//    g_print("%s",message);
//
//
//}
void add_member_list(GtkWidget *listMemberBox,int groupId)
{
    //groupId find group
    member* pmember;
    group* groupInfo;
    for(pmember = groupInfo->first_mem;pmember != groupInfo->last_mem->next ; pmember = pmember->next)
    {
        GtkWidget *memberInfo = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);

        GdkPixbuf *src = gdk_pixbuf_new_from_file(pmember->mem_pro.avatar, NULL);
        GdkPixbuf *dst = gdk_pixbuf_scale_simple(src, 20, 20, GDK_INTERP_BILINEAR);
        g_object_unref(src);
        g_object_unref(dst);
        GtkWidget *memberImg = gtk_image_new_from_pixbuf(dst);
        GtkWidget *memberlabel = gtk_label_new(pmember->mem_pro.nick_name);

        gtk_box_pack_start(GTK_BOX(memberInfo),memberImg,FALSE,FALSE,0);
        gtk_box_pack_start(GTK_BOX(memberInfo),memberlabel,FALSE,FALSE,0);



    }
}

void group_chat_window(int userId_N, int groupId_N)
{
    userId = userId_N;
    groupId = groupId_N;
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

    //gtk_init(&argc,&argv);

    //window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Group Chat Room");
    gtk_widget_set_size_request(GTK_WIDGET(window),1092,744);
    g_signal_connect(GTK_WINDOW(window),"delete_event",gtk_main_quit,NULL);

    //bigBox
    bigBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    gtk_container_add(GTK_CONTAINER(window),bigBox);

    //groupInfoBox
    groupInfoBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    gtk_box_pack_start(GTK_BOX(bigBox),groupInfoBox,TRUE,TRUE,0);
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
    gtk_widget_set_size_request(hPaned,1000,744);
    gtk_container_add(GTK_CONTAINER(bigBox),hPaned);

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
    gtk_paned_pack1(GTK_PANED(vPaned),upFrame,FALSE,FALSE);


    scrolledMessageList = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(upFrame),scrolledMessageList);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledMessageList),GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledMessageList),GTK_SHADOW_IN);

    listBox = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrolledMessageList),listBox);

    //downFrame
    downFrame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(downFrame),GTK_SHADOW_IN);
    gtk_widget_set_size_request(downFrame,-1,254);
    gtk_paned_pack2(GTK_PANED(vPaned),downFrame,TRUE,TRUE);
    //downbox
    GtkWidget *downBox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    gtk_container_add(GTK_CONTAINER(downFrame),downBox);

    //toolbar
    toolBar = gtk_toolbar_new();
    gtk_widget_set_size_request(toolBar,32,32);
    gtk_box_pack_start(GTK_BOX(downBox),toolBar,FALSE,FALSE,0);
    gtk_toolbar_set_style(GTK_TOOLBAR(toolBar),GTK_TOOLBAR_ICONS);

    GtkToolItem *iconButton = new_tool_item("../res/icons8-happy-64.png","icon");
    gtk_toolbar_insert(GTK_TOOLBAR(toolBar),iconButton,-1);
    g_signal_connect((iconButton),"clicked",G_CALLBACK(icon_button_callback),NULL);

    GtkToolItem *fileButton = new_tool_item("../res/icons8-add-file-64.png","addFile");
    gtk_toolbar_insert(GTK_TOOLBAR(toolBar),fileButton,-1);
    g_signal_connect((fileButton),"clicked",G_CALLBACK(file_button_callback),NULL);


    GtkToolItem *historyButton = new_tool_item("../res/icons8-time-machine-64.png","history");
    gtk_toolbar_insert(GTK_TOOLBAR(toolBar),historyButton,-1);
    g_signal_connect((historyButton),"clicked",G_CALLBACK(history_button_callback),NULL);


    GtkToolItem *inviteButton = new_tool_item("../res/icons8-add-user-group-man-man-64.png","invite friend");
    gtk_toolbar_insert(GTK_TOOLBAR(toolBar),inviteButton,-1);
    g_signal_connect((inviteButton),"clicked",G_CALLBACK(invite_button_callback),NULL);


    GtkToolItem *quitButton = new_tool_item("../res/icons8-exit-64.png","exit");
    gtk_toolbar_insert(GTK_TOOLBAR(toolBar),quitButton,-1);
    g_signal_connect((quitButton),"clicked",G_CALLBACK(exit_button_callback),NULL);

    //textView
    textView = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_WORD);
    gtk_box_pack_start(GTK_BOX(downBox), textView, TRUE, TRUE, 0);
    gtk_widget_grab_focus(textView);
    GtkTextBuffer *textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

    //sendButton
    sendButton = gtk_button_new_with_label("Send");
    gtk_box_pack_start(GTK_BOX(downBox),sendButton,FALSE,FALSE,0);
    g_signal_connect(G_OBJECT(sendButton),"clicked",G_CALLBACK(send_button_callback),textBuffer);

    //rightFrame
    rightFrame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(rightFrame),GTK_SHADOW_IN);
    gtk_widget_set_size_request(rightFrame,270,-1);
    gtk_paned_pack2(GTK_PANED(hPaned),rightFrame,TRUE,FALSE);
    gtk_widget_show(listBox);
    gtk_widget_show_all(window);


}




