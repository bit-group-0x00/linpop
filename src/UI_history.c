#include "../include/UI_chatView.h"
#include "../include/NET_client.h"

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
void history_msg_window (int id)
{
    friend *fri = NULL;
    group *gro = NULL;
    gchar *name = "";

    /* --- find specified id --- */
    if (id >= 10000) {
        fri = seek_fri(id);
        name = g_strdup_printf("history messages with %s", fri->fri_pro.nick_name);
    } else {
        gro = seek_gro(id);
        name = g_strdup_printf("history messages with %s", gro->gro_pro.name);
    }



    GtkWidget *historyWindow;
    GtkWidget *msgListBox;
    GtkWidget *scrolledWindow;


    /* --- Create the top level historyWindow --- */
    historyWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    /* --- Set Window Attributions ---*/

    gtk_window_set_title(GTK_WINDOW(historyWindow), name);
    gtk_window_set_position(GTK_WINDOW(historyWindow), GTK_WIN_POS_CENTER);
    gtk_widget_get_screen(historyWindow);
    gtk_window_set_default_size(GTK_WINDOW(historyWindow), 1000, 1000);
    gtk_container_set_border_width(GTK_CONTAINER(historyWindow), 20);
    gtk_widget_set_opacity(GTK_WIDGET(historyWindow), 0.95);

    /* --- You should always remember to connect the destroy event
     *     to the main historyWindow.
     */
    g_signal_connect(G_OBJECT(historyWindow), "delete_event",
                     G_CALLBACK(close_history_window), NULL);


    scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_set_border_width(GTK_CONTAINER(scrolledWindow), 10);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    /* --- Create the msgListBox --- */
    msgListBox = gtk_list_box_new();

    /* --- Set msgListBox style. --- */
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(msgListBox), GTK_SELECTION_NONE);


    /* --- Add items into the msgListBox --- */

    GtkWidget *child;
    gchar *senderName = "";
    //this is a friend
    if (id >= 10000) {
        if (fri->first_msg == NULL && fri->first_msg == fri->last_msg) {        //no message

        } else if (fri->first_msg == fri->last_msg && fri->first_msg != NULL) { //only one message
            child = getMsgBox(fri->fri_pro.nick_name, fri->first_msg->date, fri->first_msg->content);
            gtk_list_box_prepend(GTK_LIST_BOX(msgListBox), child);
        } else {
            message *p = fri->first_msg;
            while (p != fri->last_msg) {
                child = getMsgBox(fri->fri_pro.nick_name, p->date, p->content);
                gtk_list_box_prepend(GTK_LIST_BOX(msgListBox), child);
                p = p->next;
            }
            child = getMsgBox(fri->fri_pro.nick_name, p->date, p->content);
            gtk_list_box_prepend(GTK_LIST_BOX(msgListBox), child);
        }
    } else {
        if (gro->first_msg == NULL && gro->first_msg == gro->last_msg) {        //no message

        } else if (gro->first_msg == gro->last_msg && gro->first_msg != NULL) { //only one message
            senderName = g_strdup_printf("%d", gro->first_msg->sender);
            child = getMsgBox(senderName, gro->first_msg->date, gro->first_msg->content);
            gtk_list_box_prepend(GTK_LIST_BOX(msgListBox), child);
        } else {
            message *p = gro->first_msg;
            while (p != gro->last_msg) {
                senderName = g_strdup_printf("%d", p->sender);

                child = getMsgBox(senderName, p->date, p->content);
                gtk_list_box_prepend(GTK_LIST_BOX(msgListBox), child);
                p = p->next;
            }
            senderName = g_strdup_printf("%d", p->sender);
            child = getMsgBox(senderName, p->date, p->content);
            gtk_list_box_prepend(GTK_LIST_BOX(msgListBox), child);
        }
    }

    /*
     * --- Make the main historyWindow visible
     */
    gtk_container_add(GTK_CONTAINER(scrolledWindow), msgListBox);
    gtk_container_add (GTK_CONTAINER (historyWindow), scrolledWindow);
    gtk_widget_show_all (historyWindow);

}