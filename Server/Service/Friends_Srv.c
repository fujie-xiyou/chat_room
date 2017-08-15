/*************************************************************************
	>    File Name: Friends_Srv.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月12日 星期六 15时21分07秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include "Friends_Srv.h"
#include "../Persistence/Friends_Persist.h"
#include "../Persistence/Account_Persist.h"
#include "Account_Srv.h"

extern online_t* OnlineList;
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
        root = cJSON_CreateObject();
        item = cJSON_CreateString("L");
        cJSON_AddItemToObject(root ,"type" ,item);
        item = cJSON_CreateNumber(curPos -> uid);
        cJSON_AddItemToObject(root ,"uid" ,item);
        item = cJSON_CreateString(curPos -> name);
        cJSON_AddItemToObject(root ,"name" ,item);
        item = cJSON_CreateBool(curPos -> sex);
        cJSON_AddItemToObject(root ,"sex" ,item);
        item = cJSON_CreateBool(curPos -> is_vip);
        cJSON_AddItemToObject(root ,"is_vip" ,item);
        item = cJSON_CreateBool(curPos -> is_follow);
        cJSON_AddItemToObject(root ,"is_follow" ,item);
        item = cJSON_CreateBool(curPos -> is_online);
        cJSON_AddItemToObject(root ,"is_online" ,item);
        char *out = cJSON_Print(root);
        cJSON_Delete(root);
        if(send(sock_fd ,(void *)out ,strlen(out) + 1 ,0) < 0){
            perror("send 客户端响应失败");
            free(out);
            return 0;
        }
        free(out);
        usleep(50000);
    }

    //发送一个uid为0的数据告诉客户端发送完成
    root = cJSON_CreateObject();
    item = cJSON_CreateString("L");
    cJSON_AddItemToObject(root ,"type" ,item);
    item = cJSON_CreateNumber(0);
    cJSON_AddItemToObject(root ,"uid" ,item);
    char *out = cJSON_Print(root);
    cJSON_Delete(root);
    if(send(sock_fd ,(void *)out ,strlen(out) + 1 ,0) < 0){
        perror("send 客户端响应失败");
        free(out);
        return 0;
    }
    free(out);
    usleep(50000);
    //销毁链表
    List_Destroy(FriendsList ,friends_t);

    root = cJSON_CreateObject();
    item = cJSON_CreateString("R");
    cJSON_AddItemToObject(root ,"type" ,item);
    item = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root ,"res" ,item);
    out = cJSON_Print(root);
    cJSON_Delete(root);
    if(send(sock_fd ,(void *)out ,strlen(out) + 1 ,0) < 0){
        perror("recv: 客户端响应失败");
        free(out);
        return 0;
    }
    free(out);
    return 1;
}


int Friends_Srv_Add(int sock_fd ,const char *JSON){
    int f_sock_fd = -1;
    cJSON* root = cJSON_Parse(JSON);
    cJSON* item = cJSON_GetObjectItem(root ,"uid");
    int uid = item -> valueint;
    item = cJSON_GetObjectItem(root ,"fuid");
    int fuid = item -> valueint;
    cJSON_Delete(root);
    online_t *curPos;
    List_ForEach(OnlineList ,curPos){
        if(curPos -> uid == fuid) f_sock_fd = curPos -> sock_fd;
    }
    if(f_sock_fd != -1){
        //send(f_sock_fd ,
        //调整客户端去了
    }
}
