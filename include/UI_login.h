//
// Created by Anne Wu on 2019-08-30.
//
#ifndef LINPOP_UI_LOGIN_H
#define LINPOP_UI_LOGIN_H

#include <gtk/gtk.h>
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
void show_error(GtkWidget *widget, gpointer window, gchar* message);
void show_info(GtkWidget *widget, gpointer window, gchar* message);
gint show_question(GtkWidget *widget, gpointer window, gchar* message);


void test_window();
void chatView();

#endif