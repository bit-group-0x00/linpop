
#ifndef LINPOP_UI_CHATVIEW_H
#define LINPOP_UI_CHATVIEW_H

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

//用来在回调函数中传参的结构体
typedef struct data{
    GtkTextBuffer* src_buffer;
    GtkTextBuffer* destination_buffer;
}BufferDeliver;

void chat_View();
void test();



#endif //LINPOP_UI_CHATVIEW_H