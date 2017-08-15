/*************************************************************************
	>    File Name: Friends_Persist.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月12日 星期六 16时47分19秒
 ************************************************************************/

#ifndef _FRIENDS_PERSIST_H
#define _FRIENDS_PERSIST_H
#include"../Service/List.h"
#include"../Service/Friends_Srv.h"
#include<mysql/mysql.h>

int Friends_Perst_Add(int uid ,int fuid);


int Friends_Perst_GetList(friends_t * FriendsList ,int uid);


#endif
