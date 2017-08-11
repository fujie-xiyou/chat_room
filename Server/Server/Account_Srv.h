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
int Account_Srv_SignIn(int sock_fd ,char * JSON);

/*
 * 存储在线用户的uid以及对应的socket描述符
 * 带头结点的单链表结构
 */
 typedef struct online{
     int uid;
     int sock_fd;
     struct online *next;
 } online_t;


int Account_Srv_SignIn(int sock_fd , char * JSON);

int Account_Srv_Login(int sock_fd ,char * JSON);

#endif
