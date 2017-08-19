/*************************************************************************
	>    File Name: Friends_Srv.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月12日 星期六 09时46分33秒
 ************************************************************************/

#ifndef _FRIENDS_SRV_H
#define _FRIENDS_SRV_H

#include "../Common/cJSON.h"
#include "./Connect.h"
#include "../Common/List.h"


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
    int NewMsgNum; //未读消息数
    int state;
    struct friends *next;
} friends_t;


/*
 * 通过uid添加好友
 */
int Friends_Srv_SendAdd(const char *fname);

int Friends_Srv_RecvAdd(const char *JSON);

int Friends_Srv_GetList();

//用户处理好友请求
int Friends_Srv_Apply(int uid ,int fuid ,int is_agree);

//处理好友请求结果反馈
int Friends_Srv_ApplyRes(const char *JSON);

#endif
