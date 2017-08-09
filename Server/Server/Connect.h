/*************************************************************************/

#ifndef _CONNECT_H
#define _CONNECT_H
#endif

#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/type.h>
#include<netinet/in.h>
#include<"cJSON.h">


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
 * 创建并保持socket连接
 */
void Connect();


/*
 * 处理JSON消息
 */
//int
