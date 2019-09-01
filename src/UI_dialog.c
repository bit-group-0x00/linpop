//
// Created by Anne Wu on 2019-09-01.
//
#include "../include/UI_login.h"

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
    g_print("%d\n",result);
    gtk_widget_destroy(dialog);
    return result;
}
void show_info(GtkWidget *widget, gpointer window, gchar* message)
{
    //提示信息对话框
    GtkWidget *dialog;
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