/*************************************************************************
	>    File Name: Friends_Srv.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月12日 星期六 15时19分36秒
 ************************************************************************/

#ifndef _FRIENDS_SRV_H
#define _FRIENDS_SRV_H


#include "./Connect.h"


/*
 * 好友列表数据结构
 */
typedef struct friends{
    int uid;
    char name[30];
    int sex;
    int is_vip;//是否是会员
    int is_online; //是否在线
    int is_follow;//是否为特别关心
    int state;
    struct friends *next;
} friends_t;


/*
 * 获取好友信息
 */
int Friends_Srv_GetList(int sock_fd ,const char *JSON);


/*
 * 通过uid添加好友
 */
int Friends_Srv_Add(int sock_fd ,const char *JSON);

int Friends_Srv_SendAdd(int uid,int fuid ,char* type);

int Friends_Srv_Del(int sock_fd ,const char *JSON);

int Friends_Srv_Apply(int sock_fd ,const char *JSON); 

#endif
