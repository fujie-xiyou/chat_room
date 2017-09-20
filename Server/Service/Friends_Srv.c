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
#include "Account_Srv.h"
#include "Friends_Srv.h"
#include "Chat_Srv.h"
#include "../Persistence/Friends_Persist.h"
#include "../Persistence/Account_Persist.h"
#include "../Common/cJSON.h"
#include "../Common/List.h"
#define MSG_LEN 1024

extern online_t* OnlineList;
int Friends_Srv_GetList(int sock_fd ,const char *JSON){
    char buf[MSG_LEN];
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
        item = cJSON_CreateBool(curPos -> state);
        cJSON_AddItemToObject(root ,"state" ,item);
        char *out = cJSON_Print(root);
        cJSON_Delete(root);
        if(send(sock_fd ,(void *)out ,MSG_LEN ,0) < 0){
            perror("send 客户端响应失败");
            free(out);
            return 0;
        }
        free(out);
    }

    //发送一个uid为0的数据告诉客户端发送完成
    root = cJSON_CreateObject();
    item = cJSON_CreateString("L");
    cJSON_AddItemToObject(root ,"type" ,item);
    item = cJSON_CreateNumber(0);
    cJSON_AddItemToObject(root ,"uid" ,item);
    char *out = cJSON_Print(root);
    cJSON_Delete(root);
    if(send(sock_fd ,(void *)out ,MSG_LEN ,0) < 0){
        perror("send 客户端响应失败");
        free(out);
        return 0;
    }
    free(out);
    Account_Srv_SendIsOnline(uid ,1);
    //销毁链表
    List_Destroy(FriendsList ,friends_t);

    root = cJSON_CreateObject();
    item = cJSON_CreateString("R");
    cJSON_AddItemToObject(root ,"type" ,item);
    item = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root ,"res" ,item);
    out = cJSON_Print(root);
    cJSON_Delete(root);
    if(send(sock_fd ,(void *)out ,MSG_LEN ,0) < 0){
        perror("recv: 客户端响应失败");
        free(out);
        return 0;
    }
    free(out);
    Chat_Srv_SendOfflienPrivateMsg(uid);//推送离线消息
    return 1;
}


int Friends_Srv_Add(int sock_fd ,const char *JSON){
    cJSON* root = cJSON_Parse(JSON);
    cJSON* item = cJSON_GetObjectItem(root ,"uid");
    int uid = item -> valueint;
    item = cJSON_GetObjectItem(root ,"fname");
    int fuid = Account_Perst_IsUserName(item -> valuestring);
    cJSON_Delete(root);
    root = cJSON_CreateObject();
    item = cJSON_CreateString("R");
    cJSON_AddItemToObject(root ,"type" ,item);
    item = cJSON_CreateBool((fuid != 0));
    cJSON_AddItemToObject(root ,"res",item);
    if(fuid == 0){
        item = cJSON_CreateString("用户名不存在");
        cJSON_AddItemToObject(root ,"reason",item);
    }
    char *out = cJSON_Print(root);
    //printf("发给 sock_fd = %d :\n%s",sock_fd ,out);
    if(send(sock_fd ,(void *)out ,MSG_LEN ,0) <= 0){
        perror("send");
        return 0;
    }
    free(out);
    Friends_Perst_Add(uid ,fuid);
    if(Chat_Srv_GetFriendSock(fuid) == 0) return 1;
    Friends_Srv_SendAdd(uid ,fuid,"A");
    return 1;
}

int Friends_Srv_SendAdd(int uid ,int fuid ,char* type){
    int f_sock_fd = -1;
    friends_t * NewFriends = (friends_t *)malloc(sizeof(friends_t));
    if(*type == 'A'){
        NewFriends->uid = uid;
        Friends_Perst_GetFriendInfo(NewFriends);
        f_sock_fd = Chat_Srv_GetFriendSock(fuid);
    }
    else{ 
        NewFriends->uid = fuid;
        Friends_Perst_GetFriendInfo(NewFriends);
        f_sock_fd = Chat_Srv_GetFriendSock(uid);
        NewFriends -> state = 1;
    }
    cJSON *root = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString(type);
    cJSON_AddItemToObject(root ,"type" ,item);
    item = cJSON_CreateNumber(NewFriends -> uid);
    cJSON_AddItemToObject(root ,"uid" ,item);
    item = cJSON_CreateString(NewFriends -> name);
    cJSON_AddItemToObject(root ,"name" ,item);
    item = cJSON_CreateBool(NewFriends -> sex);
    cJSON_AddItemToObject(root ,"sex" ,item);
    item = cJSON_CreateBool(NewFriends -> is_vip);
    cJSON_AddItemToObject(root ,"is_vip" ,item);
    item = cJSON_CreateBool(NewFriends -> is_follow);
    cJSON_AddItemToObject(root ,"is_follow" ,item);
    item = cJSON_CreateBool(NewFriends -> is_online);
    cJSON_AddItemToObject(root ,"is_online" ,item);
    item = cJSON_CreateBool(NewFriends -> state);
    cJSON_AddItemToObject(root ,"state" ,item);
    free(NewFriends);
    char *out = cJSON_Print(root);
    cJSON_Delete(root);
    if(send(f_sock_fd ,(void *)out ,MSG_LEN,0) < 0){
        perror("send");
        printf("发给sock_fd = %d 失败\n",f_sock_fd);
        free(out);
        return 0;
    }
    free(out);
    return 1;
}

int Friends_Srv_Apply(int sock_fd ,const char *JSON){
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root ,"uid");
    int uid = item -> valueint;
    item = cJSON_GetObjectItem(root ,"fuid");
    int fuid = item -> valueint;
    item = cJSON_GetObjectItem(root ,"is_agree");
    int is_agree = item -> valueint;
    int f_sock_fd = Chat_Srv_GetFriendSock(uid);
    Friends_Perst_Apply(uid ,fuid ,is_agree);
    if(is_agree) {
        Friends_Srv_SendAdd(uid ,fuid ,"a");
    }else{
        friends_t *NewFriends = (friends_t *)malloc(sizeof(friends_t));
        NewFriends -> uid = fuid;
        Friends_Perst_GetFriendInfo(NewFriends);
        item = cJSON_CreateString(NewFriends -> name);
        cJSON_AddItemToObject(root ,"fname",item);
        char *out = cJSON_Print(root);
        cJSON_Delete(root);
        free(NewFriends);
        if(send(f_sock_fd ,(void*)out ,MSG_LEN ,0) <= 0){
            perror("send");
            return 0;
        }
        free(out);
    }
    return 1;
}
