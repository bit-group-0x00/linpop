#include "../include/UI_chatView.h"

#define CHAT_WINDOW_WIDTH 800
#define CHAT_WINDOW_HEIGHT 560
#define CHAT_BORDER_WIDTH 5

static BufferDeliver BD;

static GtkWidget *chatWindow;

//
static gint int_user_ID=0;
static gint int_friend_ID=0;

static gchar *selfID="";
static gchar *friendID="";
static gchar *selfNickname="HolyGodMT";
static gchar *friendNickname="Ironman";
static gchar *friendIP="192.158.0.1";
static gchar *friendSignature="I'm Ironman\nI killed Thanos";
static gchar *friendMsg="I'm good. And you ?";
static GtkWidget *fileChooser;
static GdkPixbuf *avatarSrc;

static GtkTextBuffer *textBuffer;
static GtkTextBuffer *msgBuffer;

void friend_msg_listener(const gchar *msg){

    //用来保存当前msgbuffer的末尾
    GtkTextIter end;

    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(msgBuffer),&end);

    gtk_text_buffer_insert(GTK_TEXT_BUFFER(msgBuffer),&end,msg,-1);

    gtk_text_buffer_insert(GTK_TEXT_BUFFER(msgBuffer),&end,"\n",-1);
}

// 按下发送按钮时发送文本框的内容至消息框,并删除文本编辑框的内容
void on_click_sendMsgBtn(GtkButton *button,gpointer data ){

    //临时字符串用来保存文本框中的信息
    gchar* tmpBuffer;

    // 消息框的Iter起点与终点
    GtkTextIter destinationStart,destinationEnd;

    //文本框的Iter起点与终点
    GtkTextIter srcStart,srcEnd;

    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textBuffer),&srcStart,&srcEnd);

    const GtkTextIter s=srcStart,e=srcEnd;

    tmpBuffer = gtk_text_buffer_get_text(textBuffer,&s,&e,FALSE);

    g_print("text input seccessful\n");

    gtk_text_buffer_get_bounds(msgBuffer,&destinationStart,&destinationEnd);

    //编辑发消息时的内容
    gtk_text_buffer_insert(msgBuffer,&destinationEnd,selfNickname,-1);

    gtk_text_buffer_insert(msgBuffer,&destinationEnd,":  ",-1);

    gtk_text_buffer_insert(msgBuffer,&destinationEnd,tmpBuffer,-1);

    gtk_text_buffer_insert(msgBuffer,&destinationEnd,"\n",-1);


    //删除文本编辑框内的内容
    gtk_text_buffer_delete(GTK_TEXT_BUFFER(textBuffer),&srcStart,&srcEnd);

}

void on_click_emojiBtn(GtkButton *button,gpointer data){

}

void on_click_sendFileBtn(GtkButton *button,gpointer data){
    GtkWidget *fileChooserDialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    const char *filename;
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

void on_click_viewHisMsgBtn(GtkButton *button,gpointer data){
    history_msg_window();
}

//infobox是一个10行40列的Box
void opera_info_box(GtkTable *infoBox){
    GtkWidget *frame;
    frame=gtk_frame_new(NULL);

    gtk_frame_set_shadow_type(GTK_FRAME(frame),GTK_SHADOW_ETCHED_IN);

    gtk_table_attach_defaults(GTK_TABLE(infoBox),frame,0,40,0,10);

    //头像,使用图片控件,来在信息框上显示头像
    GtkWidget *avatar;

    avatarSrc=gdk_pixbuf_new_from_file("../res/icon.png",NULL);

    GdkPixbuf *dst=gdk_pixbuf_scale_simple(GDK_PIXBUF(avatarSrc),120,120,GDK_INTERP_BILINEAR);

    avatar=gtk_image_new_from_pixbuf(GDK_PIXBUF(dst));

    gtk_table_attach_defaults(GTK_TABLE(infoBox),avatar,0,40,0,3);

    //利用label控件显示用户昵称
    GtkWidget *nickname;

    nickname=gtk_label_new(friendNickname);

    gtk_table_attach_defaults(GTK_TABLE(infoBox),nickname,0,40,3,4);

    //利用显示控件显示该用户的ID
    GtkWidget *ID;

    gchar *tmpID="ID: ";
    tmpID=g_strconcat(tmpID,friendID,NULL);

    ID=gtk_label_new(tmpID);

    gtk_table_attach_defaults(GTK_TABLE(infoBox),ID,0,40,4,5);

    //利用显示控件显示用户的IP地址
    GtkWidget *IP_address;

    gchar *tmpIP="IP: ";
    tmpIP=g_strconcat(tmpIP,friendIP,NULL);

    IP_address=gtk_label_new((tmpIP));

    gtk_table_attach_defaults(GTK_TABLE(infoBox),IP_address,0,40,5,6);

    //利用显示控件显示该用户的个性签名,并用frame框起来
    GtkWidget *signature;
    //个性签名中的文本内容
    GtkWidget *sigText;

    signature=gtk_frame_new("Signature");

    sigText=gtk_label_new(friendSignature);

    gtk_container_add(GTK_CONTAINER(signature),sigText);

    gtk_table_attach_defaults(GTK_TABLE(infoBox),signature,5,35,6,9);

}

//聊天界面为一个10*10的表格
void opera_chatView_box(GtkTable *msgBox){
    GtkWidget *frame;
    frame=gtk_frame_new(NULL);

    gtk_frame_set_shadow_type(GTK_FRAME(frame),GTK_SHADOW_ETCHED_IN);

    gtk_table_attach_defaults(GTK_TABLE(msgBox),frame,0,10,0,10);

}

void opera_text_input_box(GtkTable *textBox){
    GtkWidget *frame;

    frame=gtk_frame_new(NULL);

    gtk_frame_set_shadow_type(GTK_FRAME(frame),GTK_SHADOW_ETCHED_IN);

    gtk_table_attach_defaults(GTK_TABLE(textBox),frame,0,20,0,10);


    //textbox视为一个tablebox



}
// 功能盒子
void opera_function_box(GtkContainer *funcBox){
//    GtkWidget *frame;
//    frame=gtk_frame_new(NULL);
//
//    gtk_frame_set_shadow_type(GTK_FRAME(frame),GTK_SHADOW_ETCHED_IN);
//
//    gtk_container_add(GTK_CONTAINER(funcBox),frame);

    GtkWidget *emojiBtn;
    GtkWidget *sendFileBtn;
    GtkWidget *historyMsg;

    historyMsg=gtk_button_new_with_label("View history msg");

    emojiBtn=gtk_button_new_with_label("Send emoji");

    gtk_widget_set_size_request(GTK_WIDGET(emojiBtn),20,10);

    sendFileBtn=gtk_button_new_with_label("Send file");

    gtk_widget_set_size_request(GTK_WIDGET(sendFileBtn),20,10);

    gtk_widget_set_size_request(GTK_WIDGET(historyMsg),20,10);

    gtk_container_add(GTK_CONTAINER(funcBox),emojiBtn);
    gtk_container_add(GTK_CONTAINER(funcBox),sendFileBtn);
    gtk_container_add(GTK_CONTAINER(funcBox),historyMsg);

    g_signal_connect(emojiBtn,"clicked",G_CALLBACK(on_click_emojiBtn),NULL);

    g_signal_connect(sendFileBtn,"clicked",G_CALLBACK(on_click_sendFileBtn),NULL);

    g_signal_connect(historyMsg,"clicked",G_CALLBACK(on_click_viewHisMsgBtn),NULL);
}

void connect_chatView_textInput(GtkTable *msgBox,GtkTable *inputBox){
    opera_chatView_box(GTK_TABLE(msgBox));
    opera_text_input_box(GTK_TABLE(inputBox));


    //msgview
    GtkWidget *msgView;

    msgBuffer=gtk_text_buffer_new(NULL);

    msgView=gtk_text_view_new_with_buffer(msgBuffer);


    msgBuffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(msgView));

    gtk_table_attach_defaults(GTK_TABLE(msgBox),msgView,0,10,0,10);


    //设置光标不可见
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(msgView),FALSE);

    //设置不可编辑
    gtk_text_view_set_editable(GTK_TEXT_VIEW(msgView),FALSE);



    //textview
    //textbox视为一个tablebox

    GtkWidget *sendBtn;

    sendBtn=gtk_button_new_with_label("Send");

    gtk_table_attach_defaults(GTK_TABLE(inputBox),sendBtn,17,20,8,10);

    GtkWidget *text;

    textBuffer=gtk_text_buffer_new(NULL);

    text=gtk_text_view_new_with_buffer(textBuffer);

    BD.src_buffer=textBuffer;
    BD.destination_buffer=msgBuffer;

    //消息框的Iter起点与终点
    GtkTextIter msgIterStart,msgIterEnd;

    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(msgBuffer),&msgIterStart,&msgIterEnd);

    gtk_text_buffer_insert(GTK_TEXT_BUFFER(msgBuffer),&msgIterEnd,"                                                                                      Hello, life is beautiful! Let's start chatting!\n",-1);


    gtk_table_attach_defaults(GTK_TABLE(inputBox),text,0,20,1,8);

    g_signal_connect(sendBtn,"clicked",G_CALLBACK(on_click_sendMsgBtn),&BD);


}

//void test(int argc,char argv[]){
//
//    GtkWidget *window;
//    GtkWidget *ok;
//    GtkWidget *close;
//    GtkWidget *vbox;
//    GtkWidget *hbox;
//    GtkWidget *halign;
//    GtkWidget *valign;
//
//    gtk_init(&argc,&argv);
//
//    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
//
//    gtk_widget_set_size_request(GTK_WINDOW(window),WINDOW_WIDTH,WINDOW_HEIGHT);
//
//    //居中显示
//    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
//
//    //设置窗口可伸缩
//    gtk_window_set_resizable(GTK_WINDOW(window),TRUE);
//    vbox = gtk_vbox_new(FALSE,5);
//
//    valign = gtk_alignment_new(0,0,0,0);
//    gtk_container_add(GTK_CONTAINER(vbox),valign);
//    gtk_container_add(GTK_CONTAINER(window),vbox);
//
//    hbox = gtk_hbox_new(TRUE,30);
//
//
//    //设定OK和close的大小并丢到hbox里
//    ok = gtk_button_new_with_label("OK");
//    gtk_widget_set_size_request(ok,70,30);
//    gtk_container_add(GTK_CONTAINER(hbox),ok);
//    close = gtk_button_new_with_label("Close");
//    gtk_container_add(GTK_CONTAINER(hbox),close);
//
//    halign = gtk_alignment_new(1,0,0,1);
//    gtk_container_add(GTK_CONTAINER(halign),hbox);
//
//    gtk_box_pack_end(GTK_BOX(vbox),halign,FALSE,FALSE,10);
//
//    g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
//
//    gtk_widget_show_all(window);
//
//    gtk_main();
//
//
//}

void window_Layout(GtkWindow* window){

    //将整个window设置为一个表格布局
    GtkWidget *mainTable;

    mainTable=gtk_table_new(10,10,TRUE);


    gtk_container_add(GTK_CONTAINER(window),mainTable);

    //消息框设置为一个列BOX
    GtkWidget *infoBox;
    infoBox=gtk_table_new(10,40,TRUE);


    //聊天记录框
    GtkWidget *msgBox;
    msgBox=gtk_table_new(10,10,TRUE);

    //文本输入框
    GtkWidget *inputBox;
    inputBox=gtk_table_new(10,20,TRUE);

    //功能框
    GtkWidget *functionBox;
    functionBox=gtk_hbox_new(TRUE,10);

    //设置每个框所在的位置
    gtk_table_attach_defaults(GTK_TABLE(mainTable),infoBox,8,10,0,10);
    gtk_table_attach_defaults(GTK_TABLE(mainTable),msgBox,0,8,0,6);
    gtk_table_attach_defaults(GTK_TABLE(mainTable),inputBox,0,8,7,10);
    gtk_table_attach_defaults(GTK_TABLE(mainTable),functionBox,0,8,6,7);

    //分别对每个框进行装饰以及功能的嵌入

    //创建一个连接函数以实现两个控件的互动
    connect_chatView_textInput(GTK_TABLE(msgBox),GTK_TABLE(inputBox));

    opera_info_box(GTK_TABLE(infoBox));
    opera_function_box(GTK_CONTAINER(functionBox));




}

/*绘制出基本的窗口并做出分区划片*/
void chat_View(){

    //初始化

    //新建一个窗口
    chatWindow=gtk_window_new(GTK_WINDOW_TOPLEVEL);

    //设置窗口标题
    gtk_window_set_title(GTK_WINDOW(chatWindow),"chatView");

    //设置窗口大小
    gtk_widget_set_size_request(chatWindow,CHAT_WINDOW_WIDTH,CHAT_WINDOW_HEIGHT);

    //设置距离周围的宽度
    gtk_container_set_border_width(GTK_CONTAINER(chatWindow),CHAT_BORDER_WIDTH);

    //居中显示
    gtk_window_set_position(GTK_WINDOW(chatWindow),GTK_WIN_POS_CENTER);

    //设置窗口可伸缩
    gtk_window_set_resizable(GTK_WINDOW(chatWindow),TRUE);

    //对窗口进行布局
    window_Layout(GTK_WINDOW(chatWindow));

    g_signal_connect(chatWindow,"destroy",G_CALLBACK(gtk_main_quit),NULL);

    gtk_widget_show_all(chatWindow);

}

void friend_chat_window(int userID,int friend_ID){
    chat_View();
    int_user_ID=userID;
    int_friend_ID=friend_ID;

    selfID=g_strdup_printf("%d",int_user_ID);
    friendID=g_strdup_printf("%d",int_friend_ID);
}

void individual_Chat(){

}

void group_Chat(){

}


