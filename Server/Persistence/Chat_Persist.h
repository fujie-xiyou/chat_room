/*************************************************************************
	>    File Name: Chat_Persist.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月16日 星期三 09时52分31秒
 ************************************************************************/

#ifndef _CHAT_PERSIST_H
#define _CHAT_PERSIST_H
#include<mysql/mysql.h>

int Chat_Perst_Private(int from_uid ,int  to_uid ,const char *msg ,int is_offline);


int Chat_Perst_Group(int uid ,int gid ,const char *msg ,const char *offlist);

MYSQL_RES* Chat_Perst_GetOfflinePrivateMsg(int uid);

MYSQL_RES *Chat_Perst_GetPrivateRec(int uid ,int fuid);

#endif
