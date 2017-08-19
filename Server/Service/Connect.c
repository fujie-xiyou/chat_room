/************************************************************************
	>    File Name: Connect.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月09日 星期三 14时19分48秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "./Connect.h"
#include "./Account_Srv.h"
#include "./Friends_Srv.h"
#include "./Chat_Srv.h"
#include "../Common/cJSON.h"
#include "../Common/List.h"
#include "../Persistence/Friends_Persist.h"
#define LISTEN_NUM 12 //连接请求队列长度

online_t *OnlineList;
static char buf[1024];
void * thread(void *arg){
    int client_fd = (int)arg;
    while(1){
        if(recv(client_fd , (void *)buf , sizeof(buf) , 0) <= 0){
            int uid = Account_Srv_ChIsOnline(-1 , 0 ,client_fd);
            if(uid != -1){
                Account_Srv_SendIsOnline(uid ,0);
                //向在线好友发送下线通知
            }
            perror("recv");
            return NULL;
        }
        cJSON *root = cJSON_Parse(buf);
        cJSON *item = cJSON_GetObjectItem(root,"type");
        //printf("收到: sockfd = %d\n%s\n",client_fd,buf);

        switch(item -> valuestring[0]){
            case 'L' :
                //登录
                Account_Srv_Login(client_fd , buf);
                break;
            case 'S' :
                //注册
                Account_Srv_SignIn(client_fd , buf);
                break;
            case 'A' :
                //添加好友
                Friends_Srv_Add(client_fd, buf);
                break;
            case 'G' :
                //获取好友列表
                Friends_Srv_GetList(client_fd ,buf);
                break;
            case 'P' :
                //私聊
                Chat_Srv_Private(client_fd,buf);
                break;
            case 'F' :
                //文件
                Chat_Srv_File(buf);
                break;
            case 'O' :
                Account_Srv_Out(client_fd ,buf);
                break;
            case 'a':
                Friends_Srv_Apply(client_fd ,buf);
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
