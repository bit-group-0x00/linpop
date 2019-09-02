#ifndef LINPOP_UI_LOGIN_H
#define LINPOP_UI_LOGIN_H

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "UI_macro.h"


void login_window();
/**
 * 登陆界面
 *
 */
void regist_window();
/**
 * 注册界面
 *
 *
 */
void homepage_window(const int userID);
/**
 * 主界面
 *
 *
 */

void upload_file(GtkWidget *widget,
                 GdkEvent  *event,
                 gpointer   callback_data);
/**
 * 界面更新
 * @param widget 监听的对象
 * @param event event对象
 * @param callback_data 需要传递的数据
 */

void update_message(GtkWidget *widget,
                    GdkEvent  *event,
                    gpointer   callback_data);
/**
 * 界面更新
 * @param widget 监听的对象
 * @param event event对象
 * @param callback_data 需要传递的数据
 *
 */


void show_error(GtkWidget *widget, gpointer window, gchar* message);
/**
 * 错误提示
 * @param widget 监听的对象
 * @param window 传入的窗口数据
 * @param message 要展示的信息
 */
void show_info(GtkWidget *widget, gpointer window, gchar* message);
/**
 * 成功提示
 * @param widget 监听的对象
 * @param window 传入的窗口数据
 * @param message 要展示的信息
 */
gint show_question(GtkWidget *widget, gpointer window, gchar* message);
/**
 * 选择对话框
 * @param widget 监听的对象
 * @param window 传入的窗口数据
 * @param message 要展示的信息
 * @return 用户选择是返回 GTK_RESPONSE_YES，否 GTK_RESPONSE_NO
 */

#endif