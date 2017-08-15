/*************************************************************************
	>    File Name: Account_Ser.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月09日 星期三 09时34分40秒
 ************************************************************************/

#ifndef _ACCOUNT_SER_H
#define _ACCOUNT_SER_H
#include "cJSON.h"
#include "List.h"

/*
 * 存储在线用户的uid以及对应的socket描述符
 * 带头结点的单链表结构
 */
 typedef struct online{
     int uid;
     int sock_fd;
     struct online *next;
 } online_t;


/*
 * 改变用户登录状态
 * 从在线链表,数据库两方面改变
 * 如果是使用户下线,参数sock_fd可以取任意值
 */
int Account_Srv_ChIsOnline(int uid ,int is_online ,int sock_fd);

//注销登录
int Account_Srv_Out(int sock_fd , char *JSON);

int Account_Srv_SignIn(int sock_fd , char * JSON);

int Account_Srv_Login(int sock_fd ,char * JSON);

#endif
