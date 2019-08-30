#include <gtk/gtk.h>
//回调函数，data参数被忽略
void hello(GtkWidget *widget, gpointer data){
    g_print("Hello linpop");
}
//delete_event信号处理函数，后期可加入弹出你确定要退出吗的对话框
gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data){
    g_print("delete event occurred\n");
    return TRUE; //改为false程序关闭
}
//另一个回调函数
void destroy(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}
int main(int arcv, char *argv[]){
    GtkWidget *window , *button;

    gtk_init( &argc , &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(delete_event),NULL);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(destroy),NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window),10);

    button=gtk_button_new_with_label("This is a Test for linpop");

    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(hello),NULL);

    g_signal_connect_swapped(G_OBJECT(button),)

}