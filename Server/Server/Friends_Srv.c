/*************************************************************************
	>    File Name: Friends_Srv.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月12日 星期六 15时21分07秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "./Friends_Srv.h"
#include "../Persistence/Friends_Persist.h"
#include "../Persistence/Account_Persist.h"
int Friends_Srv_GetList(int sock_fd ,const char *JSON){
    char buf[1024];
    int uid;
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root ,"uid");
    uid = item -> valueint;
    cJSON_Delete(root);
    friends_t *FriendsList = NULL;
    List_Init(FriendsList ,friends_t);
    Friends_Perst_GetList(FriendsList ,uid);
    friends_t *curPos;
    List_ForEach(FriendsList ,curPos){
        Account_Perst_GetUserInfo(curPos);
        if(send(sock_fd ,curPos ,sizeof(friends_t) ,0) < 0){
            perror("send 客户端响应失败");
            return 0;
        }
    }

    //发送一个uid为0的数据告诉客户端发送完成
    FriendsList -> next ->uid = 0;
    if(send(sock_fd ,FriendsList -> next ,sizeof(friends_t) ,0) < 0){
        perror("send 客户端响应失败");
        return 0;
    }

    //销毁链表
    List_Destroy(FriendsList ,friends_t);
    root = cJSON_CreateObject();
    item = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root ,"res" ,item);
    char *out = cJSON_Print(root);
    if(recv(sock_fd ,(void *)out ,strlen(out) + 1 ,0) < 0){
        perror("recv: 客户端响应失败");
        return 0;
    }
    return 1;
}


int Friends_Srv_Add(int sock_fd ,const char *JSON){

}
