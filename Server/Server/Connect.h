/*************************************************************************/

#ifndef _CONNECT_H
#define _CONNECT_H
#endif

#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include"cJSON.h"


/*
 * socket消息结构
 * type可能的取值:
 *  L:   Login
 *  S:   Sign in
 *  C:   Chat
 *  F:   File
 */
typedef struct {
    char type;
    char msg[1023];
} msg_t;


/*
 * 存储在线用户的uid以及对应的socket描述符
 * 带头结点的单链表结构
 */
 typedef struct online{
     int uid;
     int sock_id;
     struct online *next;
 } online_t;

/*
 * 创建并保持socket连接
 */
void Connect();


/*
 * 处理JSON消息
 */
//int
