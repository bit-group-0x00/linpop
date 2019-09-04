#include "../include/UI_chatView.h"

/*
 * close_history_window
 *
 * --- Closing down window, end gtk
 */
gint close_history_window (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();

    return (FALSE);
}

/**
 * @param fromUserId
 * @return a vertical box containing time and message content
 */
GtkWidget *getMsgBox(char *fromUserName, char* time, char* content) {
    GtkWidget *nameTimeHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    GtkWidget *msgContentHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *vBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *name = gtk_label_new(fromUserName);
    GtkWidget *timeWidget = gtk_label_new(time);
    GtkWidget *messageContent = gtk_label_new(content);

    gtk_label_set_line_wrap(GTK_LABEL(messageContent), TRUE);
    gtk_box_pack_start(GTK_BOX(nameTimeHBox), name, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(nameTimeHBox), timeWidget, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(msgContentHBox), messageContent, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vBox), nameTimeHBox, FALSE, TRUE, 20);
    gtk_box_pack_start(GTK_BOX(vBox), msgContentHBox, FALSE, TRUE, 0);
    return vBox;
}
/*
 * main
 */
void history_msg_window (int argc, char *argv[])
{

}