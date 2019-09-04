#ifndef LINPOP_UI_LOGIN_H
#define LINPOP_UI_LOGIN_H

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "UI_macro.h"

extern int alreadyOpenFriendList[10000];
extern int alreadyOpenGroupList[10000];
extern GtkWidget *mainListbox;

void homepage_add_friend(GtkWidget *listbox);
void homepage_add_group(GtkWidget *listbox);
void join_group_window();
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
void add_friend_window();
/**
 * 新建群聊界面
 *
 *
 */
void friend_chat_window(int userID, int friendID);
/**
 * 好友聊天界面
 *
 *
 */
void group_chat_window(int userID, int groupID);
/**
 * 群聊界面
 *
 *
 */
void create_group_window();
/**
 * 新建群聊界面
 *
 *
 */

void homepage_window(int userID);
/**
 * 主界面
 *
 *
 */

void upload_file(int userID, int otherID, int type, int progress);
/**
 * 界面更新
 * @param userID 上传文件用户ID
 * @param userID 接受文件用户ID
 * @param type 群聊/用户
 * @param progress 聊天进度
 */

void friend_msg_listener(const gchar *msg);


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

void label_font(GtkWidget *lable,
        gchar* context, int fontSize, gchar *foreColor, gchar *underline, gchar *underlineColor);
/**
 * label控制器
 * @param context label上的文字内容
 * @param fontsize FONT_SIZE_SMALL 16 FONT_SIZE_MIDDLE 24 FONT_SIZE_BIG 16 也可以直接传数字大小
 * @param foreColor 字体颜色
 * @param backColor 背景颜色
 * @param underline 下划线类型，没有则写""
 * @param underlineColor  下划线颜色
 * label_font(passwordLabel,"Password",FONT_SIZE_MIDDLE,"black","double","blue");
 *  none/single/double/low/error
 */
void history_msg_window(int id);


#endif