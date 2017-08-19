/*************************************************************************
	>    File Name: Chat_Srv.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月16日 星期三 10时37分36秒
 ************************************************************************/

#ifndef _CHAT_SRV_H
#define _CHAT_SRV_H

void Chat_Srv_File(const char *JSON);

int Chat_Srv_GetFriendSock(int fuid);

int Chat_Srv_Private(int sock_fd ,const char *JSON);

int Chat_Srv_Group(int sock_fd ,const char *JSON);

#endif
