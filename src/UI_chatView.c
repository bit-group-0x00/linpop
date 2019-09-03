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

/*测试函数*/
void func(int argc,char *argv[])
{
    //1.gtk环境初始化
    gtk_init(&argc, &argv);

    //2.创建一个窗口
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    //3.创建一个表格容器5行4列
    GtkWidget *table = gtk_table_new(5,4,TRUE);
    //将table加入到window中
    gtk_container_add(GTK_CONTAINER(window), table);

    //4.创建一个行编辑
    GtkWidget *entry = gtk_entry_new();
    //设置行编辑的内容
    gtk_entry_set_text(GTK_ENTRY(entry), "");
    //设置行编辑不允许编辑，只能显示用
    gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);

    //5.创建多个按钮
    GtkWidget *button0 = gtk_button_new_with_label("0");//数值键0
    GtkWidget *button1 = gtk_button_new_with_label("1");//数值键1
    GtkWidget *button2 = gtk_button_new_with_label("2");//数值键2
    GtkWidget *button3 = gtk_button_new_with_label("3");//数值键3
    GtkWidget *button4 = gtk_button_new_with_label("4");//数值键4
    GtkWidget *button5 = gtk_button_new_with_label("5");//数值键5
    GtkWidget *button6 = gtk_button_new_with_label("6");//数值键6
    GtkWidget *button7 = gtk_button_new_with_label("7");//数值键7
    GtkWidget *button8 = gtk_button_new_with_label("8");//数值键8
    GtkWidget *button9 = gtk_button_new_with_label("9");//数值键9

    GtkWidget *button_add = gtk_button_new_with_label("+");//加号
    GtkWidget *button_minus = gtk_button_new_with_label("-");//减号
    GtkWidget *button_mul = gtk_button_new_with_label("*");//乘号
    GtkWidget *button_div = gtk_button_new_with_label("/");//除号
    GtkWidget *button_equal = gtk_button_new_with_label("=");//等号
    GtkWidget *button_delete = gtk_button_new_with_label("c");//退格键

    //6.布局将上面的按钮均放入table容器中
    gtk_table_attach_defaults(GTK_TABLE(table), entry, 0, 4, 0, 1);

    gtk_table_attach_defaults(GTK_TABLE(table), button0, 0, 1, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(table), button1, 0, 1, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table), button2, 1, 2, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table), button3, 2, 3, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table), button4, 0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), button5, 1, 2, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), button6, 2, 3, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), button7, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), button8, 1, 2, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), button9, 2, 3, 1, 2);

    gtk_table_attach_defaults(GTK_TABLE(table), button_add, 1, 2, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(table), button_minus, 2, 3, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(table), button_mul , 3, 4, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), button_div, 3, 4, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table), button_equal, 3, 4, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(table), button_delete, 3, 4, 1, 2);

    //7.注册信号函数，把entry传给回调函数deal_num()
    g_signal_connect(button0, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button1, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button2, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button3, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button4, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button5, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button6, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button7, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button8, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button9, "pressed", G_CALLBACK(deal_num), entry);

    g_signal_connect(button_add, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button_mul, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button_div, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button_minus, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button_equal, "pressed", G_CALLBACK(deal_num), entry);
    g_signal_connect(button_delete, "pressed", G_CALLBACK(deal_num), entry);

    //7.显示所有控件
    gtk_widget_show_all(window);

    //8.主事件循环
    gtk_main();

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
//    gtk_fixed_set_has_window(fixed_info,TRUE);

    GtkWidget *verticalBox_info=gtk_vbox_new(TRUE,10);
    gtk_container_add(GTK_CONTAINER(fixed_info),verticalBox_info);
    gtk_fixed_put(GTK_FIXED(fixed_info),verticalBox_info,20,20);

    //创建一个除去info框的Default固定框
    GtkWidget *fixed_default=gtk_fixed_new();
    gtk_container_add(GTK_FIXED(fixed),fixed_default);
    gtk_fixed_put(GTK_FIXED(fixed),fixed_default,20,20);
//    gtk_fixed_set_has_window(fixed_default,TRUE);

    //在chatTable也就是聊天框中加入demo
    //其中文本输入框为fixed布局,历史消息显示框为layout布局

    //打字框
    GtkWidget *fixed_typing=gtk_fixed_new();
    gtk_fixed_put(GTK_FIXED(fixed_default),fixed_typing,0,height-20-typingBox_height);
//    gtk_fixed_set_has_window(fixed_typing,TRUE);

    //表情等功能框
    GtkWidget *fixed_function=gtk_fixed_new();
    gtk_fixed_put(GTK_FIXED(fixed_default),fixed_function,0,height-20-typingBox_height-functionBox_height);
//    gtk_fixed_set_has_window(fixed_function,TRUE);

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


