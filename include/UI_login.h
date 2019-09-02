//
// Created by Anne Wu on 2019-08-30.
//
#ifndef LINPOP_UI_LOGIN_H
#define LINPOP_UI_LOGIN_H

#include <gtk/gtk.h>
#include "UI_macro.h"



void loginWindow();
/**
 * 登陆界面
 *
 */
void registWindow();
/**
 * 注册界面
 *
 *
 */
void homepageWindow();


void addFriend();
/*
 * 添加好友
 *
 *
 */

gint show_question(GtkWidget *widget, gpointer window, gchar* message);

void show_error(GtkWidget *widget, gpointer window, gchar* message);
/**
 *  选择对话框，返回用户选项

 */
void show_info(GtkWidget *widget, gpointer window, gchar* message);

#endif