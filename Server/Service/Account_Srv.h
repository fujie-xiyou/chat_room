/*************************************************************************
	>    File Name: Account_Ser.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月09日 星期三 09时34分40秒
 ************************************************************************/

#ifndef _ACCOUNT_SER_H
#define _ACCOUNT_SER_H
#include "./Friends_Srv.h"
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
 * 如果是使用户下线,参数uid可以取任意值
 * 上线成功返回1 失败返回0
 * 下线成功返回uid  失败返回0
 */
int Account_Srv_ChIsOnline(int uid ,int is_online ,int sock_fd);

//向某个好友发送上下线提示
int Account_Srv_SendIsOnline(int uid, int is_online);

//注销登录
int Account_Srv_Out(int sock_fd , char *JSON);

int Account_Srv_SignIn(int sock_fd , char * JSON);

int Account_Srv_Login(int sock_fd ,char * JSON);

#endif
