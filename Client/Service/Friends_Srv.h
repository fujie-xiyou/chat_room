/*************************************************************************
	>    File Name: Friends_Srv.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月12日 星期六 09时46分33秒
 ************************************************************************/

#ifndef _FRIENDS_SRV_H
#define _FRIENDS_SRV_H

#include "./cJSON.h"
#include "./Connect.h"
#include "./List.h"


/*
 * 好友列表数据结构
 */
typedef struct friends{
    int uid;
    char name[30];
    int sex;
    int is_vip;//是否是会员
    int is_follow;//是否为特别关心
    int is_online; //是否在线
    struct friends *next;
} friends_t;


/*
 * 通过uid添加好友
 */
int Friends_Srv_Add(int fuid);

#endif
