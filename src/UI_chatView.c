#include "../include/UI_chatView.h"
#include "../include/NET_client.h"

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
static gchar *selfNickname="";
static gchar *friendNickname="";
static gchar *friendIP="";
static gchar *friendSignature="";
static gchar *friendMsg="";
static GtkWidget *fileChooser;
static GdkPixbuf *avatarSrc;

static GtkTextBuffer *textBuffer;
static GtkTextBuffer *msgBuffer;

static gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data){
//    alreadyOpenList[friend_ID-10000]=FALSE;
    gint state;
    g_print("delete event occured\n");
    gtk_main_quit();

    GtkTextIter start,end;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textBuffer),&start,&end);

    gtk_text_buffer_delete(GTK_TEXT_BUFFER(textBuffer),&start,&end);
    return FALSE;
}

void handle_data(){
    friend *pfriend;
    friend *pself;

    pfriend=seek_fri(int_friend_ID);
    pself=seek_fri(int_user_ID);

    friendIP=pfriend->fri_pro.ip;
    friendSignature=pfriend->fri_pro.signature;
    friendNickname=pfriend->fri_pro.nick_name;

    gchar *filename;

    filename=pfriend->fri_pro.avatar;
    avatarSrc=gdk_pixbuf_new_from_file(filename,NULL);
}

void friend_msg_listener(const gchar *msg){

    GtkTextIter end;

    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(msgBuffer),&end);

    gtk_text_buffer_insert(GTK_TEXT_BUFFER(msgBuffer),&end,friendNickname,-1);

    gtk_text_buffer_insert(GTK_TEXT_BUFFER(msgBuffer),&end,": ",-1);

    gtk_text_buffer_insert(GTK_TEXT_BUFFER(msgBuffer),&end,msg,-1);

    gtk_text_buffer_insert(GTK_TEXT_BUFFER(msgBuffer),&end,"\n",-1);


}
void friend_msg_show( ){

    GtkTextIter end;

    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(msgBuffer),&end);

    gtk_text_buffer_insert(GTK_TEXT_BUFFER(msgBuffer),&end,friendMsg,-1);

    gtk_text_buffer_insert(GTK_TEXT_BUFFER(msgBuffer),&end,"\n",-1);


}

void on_click_sendMsgBtn(GtkButton *button,gpointer data ){

    //��ʱ�ַ������������ı����е���Ϣ
    gchar* tmpBuffer;

    // ��Ϣ����Iter�������յ�
    GtkTextIter destinationStart,destinationEnd;

    //�ı�����Iter�������յ�
    GtkTextIter srcStart,srcEnd;

    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textBuffer),&srcStart,&srcEnd);

    const GtkTextIter s=srcStart,e=srcEnd;

    tmpBuffer = gtk_text_buffer_get_text(textBuffer,&s,&e,FALSE);

    g_print("text input seccessful\n");

    gtk_text_buffer_get_bounds(msgBuffer,&destinationStart,&destinationEnd);

    //�༭����Ϣʱ������
    gtk_text_buffer_insert(msgBuffer,&destinationEnd,selfNickname,-1);

    gtk_text_buffer_insert(msgBuffer,&destinationEnd,":  ",-1);

    gtk_text_buffer_insert(msgBuffer,&destinationEnd,tmpBuffer,-1);

    gtk_text_buffer_insert(msgBuffer,&destinationEnd,"\n",-1);

    send_msg_to_friend(int_friend_ID,tmpBuffer);

    free(tmpBuffer);
    //ɾ���ı��༭���ڵ�����
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

//infobox��һ��10��40�е�Box
void opera_info_box(GtkTable *infoBox){
    GtkWidget *frame;
    frame=gtk_frame_new(NULL);

    gtk_frame_set_shadow_type(GTK_FRAME(frame),GTK_SHADOW_ETCHED_IN);

    gtk_table_attach_defaults(GTK_TABLE(infoBox),frame,0,40,0,10);

    //ͷ��,ʹ��ͼƬ�ؼ�,������Ϣ������ʾͷ��
    GtkWidget *avatar;

    avatarSrc=gdk_pixbuf_new_from_file("../res/icon.png",NULL);

    GdkPixbuf *dst=gdk_pixbuf_scale_simple(GDK_PIXBUF(avatarSrc),120,120,GDK_INTERP_BILINEAR);

    avatar=gtk_image_new_from_pixbuf(GDK_PIXBUF(dst));

    gtk_table_attach_defaults(GTK_TABLE(infoBox),avatar,0,40,0,3);

    //����label�ؼ���ʾ�û��ǳ�
    GtkWidget *nickname;

    nickname=gtk_label_new(friendNickname);

    gtk_table_attach_defaults(GTK_TABLE(infoBox),nickname,0,40,3,4);

    //������ʾ�ؼ���ʾ���û���ID
    GtkWidget *ID;

    gchar *tmpID="ID: ";
    tmpID=g_strconcat(tmpID,friendID,NULL);

    ID=gtk_label_new(tmpID);

    gtk_table_attach_defaults(GTK_TABLE(infoBox),ID,0,40,4,5);

    //������ʾ�ؼ���ʾ�û���IP��ַ
    GtkWidget *IP_address;

    gchar *tmpIP="IP: ";
    tmpIP=g_strconcat(tmpIP,friendIP,NULL);

    IP_address=gtk_label_new((tmpIP));

    gtk_table_attach_defaults(GTK_TABLE(infoBox),IP_address,0,40,5,6);

    //������ʾ�ؼ���ʾ���û��ĸ���ǩ��,����frame������
    GtkWidget *signature;
    //����ǩ���е��ı�����
    GtkWidget *sigText;

    signature=gtk_frame_new("Signature");

    sigText=gtk_label_new(friendSignature);

    gtk_container_add(GTK_CONTAINER(signature),sigText);

    gtk_table_attach_defaults(GTK_TABLE(infoBox),signature,5,35,6,9);

}

//��������Ϊһ��10*10�ı���
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


    //textbox��Ϊһ��tablebox



}
// ���ܺ���
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


    //���ù��겻�ɼ�
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(msgView),FALSE);

    //���ò��ɱ༭
    gtk_text_view_set_editable(GTK_TEXT_VIEW(msgView),FALSE);



    //textview
    //textbox��Ϊһ��tablebox

    GtkWidget *sendBtn;

    sendBtn=gtk_button_new_with_label("Send");

    gtk_table_attach_defaults(GTK_TABLE(inputBox),sendBtn,17,20,8,10);

    GtkWidget *text;

    textBuffer=gtk_text_buffer_new(NULL);

    text=gtk_text_view_new_with_buffer(textBuffer);

    BD.src_buffer=textBuffer;
    BD.destination_buffer=msgBuffer;

    //��Ϣ����Iter�������յ�
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
//    //������ʾ
//    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
//
//    //���ô��ڿ�����
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
//    //�趨OK��close�Ĵ�С������hbox��
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

    //������window����Ϊһ�����񲼾�
    GtkWidget *mainTable;

    mainTable=gtk_table_new(10,10,TRUE);


    gtk_container_add(GTK_CONTAINER(window),mainTable);

    //��Ϣ������Ϊһ����BOX
    GtkWidget *infoBox;
    infoBox=gtk_table_new(10,40,TRUE);


    //������¼��
    GtkWidget *msgBox;
    msgBox=gtk_table_new(10,10,TRUE);

    //�ı�������
    GtkWidget *inputBox;
    inputBox=gtk_table_new(10,20,TRUE);

    //���ܿ�
    GtkWidget *functionBox;
    functionBox=gtk_hbox_new(TRUE,10);

    //����ÿ�������ڵ�λ��
    gtk_table_attach_defaults(GTK_TABLE(mainTable),infoBox,8,10,0,10);
    gtk_table_attach_defaults(GTK_TABLE(mainTable),msgBox,0,8,0,6);
    gtk_table_attach_defaults(GTK_TABLE(mainTable),inputBox,0,8,7,10);
    gtk_table_attach_defaults(GTK_TABLE(mainTable),functionBox,0,8,6,7);

    //�ֱ���ÿ��������װ���Լ����ܵ�Ƕ��

    //����һ�����Ӻ�����ʵ�������ؼ��Ļ���
    connect_chatView_textInput(GTK_TABLE(msgBox),GTK_TABLE(inputBox));

    opera_info_box(GTK_TABLE(infoBox));
    opera_function_box(GTK_CONTAINER(functionBox));




}

/*���Ƴ������Ĵ��ڲ�����������Ƭ*/
void chat_View(){

    //��ʼ��

    //�½�һ������
    chatWindow=gtk_window_new(GTK_WINDOW_TOPLEVEL);
//
//    //���ô��ڱ���
    gtk_window_set_title(GTK_WINDOW(chatWindow),"chatView");
//
//    //���ô��ڴ�С
    gtk_widget_set_size_request(chatWindow,CHAT_WINDOW_WIDTH,CHAT_WINDOW_HEIGHT);
//
//    //���þ�����Χ�Ŀ���
    gtk_container_set_border_width(GTK_CONTAINER(chatWindow),CHAT_BORDER_WIDTH);
//
//    //������ʾ
    gtk_window_set_position(GTK_WINDOW(chatWindow),GTK_WIN_POS_CENTER);
//
//    //���ô��ڿ�����
    gtk_window_set_resizable(GTK_WINDOW(chatWindow),TRUE);
//
//    //�Դ��ڽ��в���
    window_Layout(GTK_WINDOW(chatWindow));

    g_signal_connect(chatWindow,"delete_event",G_CALLBACK(delete_event),NULL);

    gtk_widget_show_all(chatWindow);
    gtk_main();

}

void friend_chat_window(int userID,int friend_ID){
    int_user_ID=userID;
    int_friend_ID=friend_ID;
    selfID=g_strdup_printf("%d",int_user_ID);
    friendID=g_strdup_printf("%d",int_friend_ID);
    handle_data();
    chat_View();
}

void individual_Chat(){

}

void group_Chat(){

}


