//
// Created by Anne Wu on 2019-08-30.
//
#include "../include/UI_chatView.h"

char buf[50];
void deal_num(GtkButton *button, gpointer data)
{
    const char *text = gtk_button_get_label(button);

    //退个操作
    if (0 == strcmp(text, "c")) {
        buf[strlen(buf) - 1] = 0;
    }
    else
    {
        int a = 0, b = 0;
        char c;
        strcat(buf,text);

        if (0 == strcmp("=", text)) {
            printf("text==##%s##\n",text);
            sscanf(buf,"%d%c%d", &a, &c, &b);

            printf("---------001-----%c--\n",c);
            if ('+' == c) {
                sprintf(buf,"%d", a+b);
            }
            else if ('-' == c) {
                sprintf(buf, "%d", a-b);
            }
            else if ('*' == c) {
                sprintf(buf, "%d", a*b);
            }
            else if ('/' == c) {
                sprintf(buf, "%d", a/b);
            }

        }
    }

    gtk_entry_set_text(GTK_ENTRY(data), buf);

    return;
}
void windowLayout(GtkWindow* window){
    //窗口的高度和宽度
    gint width,height;
    //信息框的宽度
    gint infoBox_width=130;
    //打字框的高度
    gint typingBox_height=130;
    //功能框的高度
    gint functionBox_height=20;

    gtk_window_get_size(window,width,height);

    // 在window中创建一个固定布局以往里面放更多固定布局
    GtkWidget *fixed=gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window),fixed);

    //个人信息框是一个fixed布局,但里面的内容遵循垂直布局
    GtkWidget *fixed_info=gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(fixed),fixed_info);
    gtk_fixed_put(GTK_FIXED(fixed),fixed_info,width-infoBox_width,20);
    gtk_fixed_set_has_window(fixed_info,TRUE);

    GtkWidget *verticalBox_info=gtk_vbox_new(TRUE,10);
    gtk_container_add(GTK_CONTAINER(fixed_info),verticalBox_info);
    gtk_fixed_put(GTK_FIXED(fixed_info),verticalBox_info,20,20);

    //创建一个除去info框的Default固定框
    GtkWidget *fixed_default=gtk_fixed_new();
    gtk_container_add(GTK_FIXED(fixed),fixed_default);
    gtk_fixed_put(GTK_FIXED(fixed),fixed_default,20,20);
    gtk_fixed_set_has_window(fixed_default,TRUE);

    //在chatTable也就是聊天框中加入demo
    //其中文本输入框为fixed布局,历史消息显示框为layout布局

    //打字框
    GtkWidget *fixed_typing=gtk_fixed_new();
    gtk_fixed_put(GTK_FIXED(fixed_default),fixed_typing,0,height-20-typingBox_height);
    gtk_fixed_set_has_window(fixed_typing,TRUE);

    //表情等功能框
    GtkWidget *fixed_function=gtk_fixed_new();
    gtk_fixed_put(GTK_FIXED(fixed_default),fixed_function,0,height-20-typingBox_height-functionBox_height);
    gtk_fixed_set_has_window(fixed_function,TRUE);

    //历史消息框
    //GtkWidget *layout_history=gtk_layout_new()



}

/*绘制出基本的窗口并做出分区划片*/
void chatView(int argc, char *argv[]){


    GtkWidget *window;

    //初始化
    gtk_init(&argc,&argv);

    //新建一个窗口
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

    //设置窗口标题
    gtk_window_set_title(GTK_WINDOW(window),"chatView");

    //设置窗口大小
    gtk_widget_set_size_request(GTK_WINDOW(window),800,560);

    //居中显示
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    //设置窗口可伸缩
    gtk_window_set_resizable(GTK_WINDOW(window),FALSE);

    //对窗口进行布局
    windowLayout(GTK_WINDOW(window));

    g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);

    gtk_widget_show_all(window);

    gtk_main();

}


void individualChat(){

}

void groupChat(){

}


