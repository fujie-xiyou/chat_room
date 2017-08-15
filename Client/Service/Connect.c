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
#include "Connect.h"
#include "cJSON.h"

int sock_fd;
pthread_mutex_t recv_mutex;
char massage[1024];
void * thread(void *arg){
    while(1){
        //printf("线程上锁之前\n");
        pthread_mutex_lock(&recv_mutex);
        //printf("线程上锁之后\n");
        if(recv(sock_fd ,massage ,sizeof(massage) ,0) <= 0){
            perror("recv: 服务器失去响应");
            exit(0);
        }
        //printf("收到:%s\n",massage);
        cJSON *root = cJSON_Parse(massage);
        cJSON *item = cJSON_GetObjectItem(root ,"type");
        switch(item -> valuestring[0]){
            case 'A' :
                //处理好友请求
                break;
            case 'C' :
                //处理私聊消息
                break;
            case 'F' :
                //处理文件请求
                break;
            case 'Q' :
                //群聊
                break;
            case 'R' :
                //说明此条消息是主线程请求的结果反馈
                //因此不做处理,等待主线程处理
                break;
            case 'L' :
                //获取好友列表
                //主线程处理
                break;
        }
        //printf("线程解锁之前\n");
        pthread_mutex_unlock(&recv_mutex);
        //printf("线程解锁之后\n");
        usleep(5000);
    }
    return NULL;
}
void Connect(){
    struct sockaddr_in clie_addr;
    //char *out;
    pthread_t thid;
    pthread_mutex_init(&recv_mutex ,NULL);
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
}
