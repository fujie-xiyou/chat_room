/*************************************************************************
	>    File Name: Chat_Srv.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月16日 星期三 10时58分52秒
 ************************************************************************/

#ifndef _CHAT_SRV_H
#define _CHAT_SRV_H
#include "../Common/Common.h"
typedef struct private_msg {
    int from_uid;
    char name[30];
    char msg[1000];
    char time [25];
    struct private_msg *next;
}private_msg_t;

typedef struct group_msg {
    int from_uid;
    int gid;
    char uname[30];
    char gname[30];
    char msg[1000];
    char time [25];
    struct group_msg *next;
}group_msg_t;


void Chat_Srv_InitList();

void Chat_Srv_RecvPrivate(const char *JSON);

int Chat_Srv_SendPrivate(int to_uid ,const char *msg);

void Chat_Srv_RecvGroup(const char *JSON);

int Chat_Srv_SendGroup(int to_uid ,const char *msg);

int Chat_Srv_SendFile(const char *filename ,int fuid);

int Chat_Srv_RecvFile(const char *JSON);

void Chat_Srv_ShowPrivateRec(const char *JSON);

void Chat_Srv_GetPrivateRec(int fuid);

#endif
