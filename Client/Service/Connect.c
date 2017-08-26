/*************************************************************************
	>    File Name: Connect.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月10日 星期四 17时42分08秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "./Connect.h"
#include "./Chat_Srv.h"
#include "./Account_Srv.h"
#include "./Friends_Srv.h"
#include "../Common/cJSON.h"
#define MSG_LEN 1024

int sock_fd;
//pthread_mutex_t mutex;
//pthread_cond_t cond;
extern int my_mutex;
char massage[MSG_LEN];

void * thread(void *arg){
    int ret ,recv_len;
    cJSON *root ,*item;
    while(1){
        /*
        printf("线程上锁之前\n");
        pthread_mutex_lock(&mutex);
        printf("线程上锁之后\n");
        pthread_cond_wait(&cond ,&mutex);
        printf("线程条件变量为真\n");
        */
        if(my_mutex) continue;
        //printf("等待接收\n");
        recv_len = 0;
        while(recv_len < MSG_LEN){
            ret = 0;
            if((ret = recv(sock_fd ,massage ,MSG_LEN - recv_len,0)) <= 0){
                perror("recv: 服务器失去响应");
                exit(0);
            }
            recv_len += ret;
        }
        //printf("收到:%s\n",massage);
        root = cJSON_Parse(massage);
        item = cJSON_GetObjectItem(root ,"type");
        switch(item -> valuestring[0]){
            case 'A' :
                Friends_Srv_RecvAdd(massage);
                //处理好友请求
                break;
            case 'a':
                //处理好友请求反馈
                Friends_Srv_ApplyRes(massage);
                break;
            case 'P' :
                //处理私聊消息
                Chat_Srv_RecvPrivate(massage);
                break;
            case 'F' :
                Chat_Srv_RecvFile(massage);
                //处理文件请求
                break;
            case 'Q' :
                //群聊
                break;
            case 'R' :
                //说明此条消息是主线程请求的结果反馈
                //因此不做处理,等待主线程处理
                my_mutex = 1;
                break;
            case 'L' :
                my_mutex = 1;
                //获取好友列表
                //主线程处理
                break;
            case 'I' :
                //好友上下线请求
                Account_Srv_RecvIsOnline(massage);
        }
        /*
        printf("线程解锁之前\n");
        pthread_mutex_unlock(&mutex);
        printf("线程解锁之后\n");
        pthread_cond_signal(&cond);
        //usleep(5000);
        */
    }
    return NULL;
}
void Connect(){
    struct sockaddr_in clie_addr;
    //char *out;
    pthread_t thid;
 //   pthread_mutex_init(&mutex ,NULL);
   // pthread_cond_init(&cond ,NULL);
    memset(&clie_addr , 0 ,sizeof(struct sockaddr_in));
    clie_addr.sin_family = AF_INET;
    clie_addr.sin_port = htons(1314);
    clie_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_fd = socket(AF_INET , SOCK_STREAM , 0);
    if(sock_fd < 0){
        perror("socket");
        exit(0);
    }
    if(connect(sock_fd , (struct sockaddr *)&clie_addr,sizeof(struct sockaddr_in)) < 0){
        perror("connect");
        exit(0);
    }
    pthread_create(&thid ,NULL ,thread ,NULL);
    //usleep(5000);
    //pthread_cond_signal(&cond);
}
