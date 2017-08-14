/************************************************************************
	>    File Name: Connect.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月09日 星期三 14时19分48秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Connect.h"
#include <unistd.h>
#include <pthread.h>
#include "List.h"
#include "Account_Srv.h"
#define LISTEN_NUM 12 //连接请求队列长度

online_t *OnlineList;
static char buf[1024];
void * thread(void *arg){
    int client_fd = (int)arg;
    while(1){
        if(recv(client_fd , (void *)buf , sizeof(buf) , 0) <= 0){
            perror("recv");
            //exit(0);
            break;
        }
        cJSON *root = cJSON_Parse(buf);
        cJSON *item = cJSON_GetObjectItem(root,"type");
        switch(item -> valuestring[0]){
            case 'L' :
                //登录
                item = cJSON_GetObjectItem(root,"data");
                Account_Srv_Login(client_fd , item -> valuestring);
                break;
            case 'S' :
                //注册
                item = cJSON_GetObjectItem(root,"data");
                Account_Srv_SignIn(client_fd , item -> valuestring);
                break;
            case 'A' :
                //添加好友
                item = cJSON_GetObjectItem(root,"data");
                //Friends_Srv_Add(client_fd, item -> valuestring);
                break;
            case 'G' :
                //获取好友列表
                item = cJSON_GetObjectItem(root ,"data");
                //Friends_Srv_GetList(client_fd ,item -> valuestring);
            case 'C' :
                //聊天
                item = cJSON_GetObjectItem(root,"data");
                //Chat_Srv_(item -> valuestring);
                break;
            case 'F' :
                //文件
                item = cJSON_GetObjectItem(root,"data");
                printf("敬请期待\n");
                break;
        }
    }
    return NULL;
}


void Connect(){
    int sock_fd;
    int client_fd;
    int len;
    int optval;
    List_Init(OnlineList , online_t);
    struct sockaddr_in serv_addr , client_addr;
    len = sizeof(struct sockaddr_in);
    memset(&serv_addr , 0 ,len);
    memset(&client_addr , 0 , len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(1314);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_fd = socket(AF_INET , SOCK_STREAM , 0);
    if(sock_fd < 0) {
        perror("socket");
        exit(0);
    }
    optval = 1;
    if(setsockopt(sock_fd , SOL_SOCKET , SO_REUSEADDR , (void *)&optval , sizeof(int)) < 0){
        perror("socksetopt");
        exit(0);
    }
    if(bind(sock_fd , (struct sockaddr *)&serv_addr , len) < 0){
        perror("bind");
        exit(0);
    }
    if(listen(sock_fd , LISTEN_NUM) < 0){
        perror("listen");
        exit(0);
    }
    while(1){
        client_fd = accept(sock_fd , (struct sockaddr *)&client_addr , (socklen_t *)&len);
        if(client_fd < 0) {
            perror("accept");
            exit(0);
        }
        pthread_t thid;
        pthread_create(&thid , NULL , thread ,(void *)client_fd);
    }
    
}
