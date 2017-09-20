/*************************************************************************
	>    File Name: Account_Srv.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月10日 星期四 17时31分06秒
 ************************************************************************/

#include <stdio.h>
#include "Account_Srv.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "./Connect.h"
#include "./Friends_Srv.h"
#include "../Common/List.h"

extern int sock_fd;
//extern pthread_mutex_t mutex;
//extern pthread_cond_t cond;
extern int my_mutex;
extern char massage[1024];
extern friends_t *FriendsList;
extern int gl_uid;

void Account_Srv_RecvIsOnline(char *JSON){
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root ,"is_online");
    int is_online = item -> valueint;
    item = cJSON_GetObjectItem(root ,"fuid");
    int fuid = item -> valueint;
    friends_t *f;
    List_ForEach(FriendsList ,f){
        if(f -> uid == fuid) {
            f -> is_online = is_online;
            if(is_online)
                printf("\n%s 上线啦!\n" ,f -> name);
            else
                printf("\n%s 已下线.\n" ,f -> name);
        }
    }
}
/*
 * 注销登录
 */
int Account_Srv_Out(int uid){
    int rtn;
    cJSON *root = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString("O");
    cJSON_AddItemToObject(root ,"type",item);
    item = cJSON_CreateNumber(uid);
    cJSON_AddItemToObject(root , "uid" ,item);
    char *out = cJSON_Print(root);
    if(send(sock_fd ,(void *)out ,1024 ,0) <= 0){
        perror("send 请求服务器失败");
        rtn = 0;
    }
    gl_uid = 0;
    cJSON_Delete(root);
    free(out);
    //pthread_mutex_lock(&mutex);
    //pthread_cond_wait(&cond ,&mutex);
    My_Lock();
    root = cJSON_Parse(massage);
    item = cJSON_GetObjectItem(root ,"res");
    if(item -> valueint == 0){
        item = cJSON_GetObjectItem(root ,"reason");
        printf("注销失败: %s",item -> valuestring);
        rtn = 0;
    }else{
        printf("注销成功,按任意键继续..");
        rtn = 1;
    }
    cJSON_Delete(root);
    My_Unlock();
    getchar();
    //pthread_mutex_unlock(&mutex);
    return rtn;
}

int Account_Srv_SignIn(const char * name ,int sex ,const char * password){
    //char buf[1024];
    int rtn;
    cJSON *root = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString("S");
    cJSON_AddItemToObject(root,"type",item);
    item = cJSON_CreateString(name);
    cJSON_AddItemToObject(root,"name",item);
    item = cJSON_CreateBool(sex);
    cJSON_AddItemToObject(root ,"sex" ,item);
    item = cJSON_CreateString(password);
    cJSON_AddItemToObject(root,"password",item);
    char *out = cJSON_Print(root);
    if(send(sock_fd , (void *)out , 1024 ,0) < 0 ){
        perror("send: 请求服务器失败");
        return 0;
    }
    free(out);
    cJSON_Delete(root);
    //pthread_mutex_lock(&mutex);
    //pthread_cond_wait(&cond ,&mutex);
    My_Lock();
    root = cJSON_Parse(massage);
    item = cJSON_GetObjectItem(root,"res");
    int res = item -> valueint;
    if(res == 1) {
        printf("注册成功!按任意键继续");
        getchar();
        rtn = 1;
    }else{
        item = cJSON_GetObjectItem(root,"reason");
        printf("注册失败: %s",item -> valuestring);
        getchar();
        rtn = 0;
    }
    cJSON_Delete(root);
    //pthread_mutex_unlock(&mutex);
    My_Unlock();
    return rtn;
}
int Account_Srv_Login(const char *name , const char *password){
    //printf("进入登录函数\n");
    //char buf[1024];
    int rtn;
    cJSON *root = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString("L");
    cJSON_AddItemToObject(root,"type",item);
    item = cJSON_CreateString(name);
    cJSON_AddItemToObject(root,"name",item);
    item = cJSON_CreateString(password);
    cJSON_AddItemToObject(root,"password",item);
    char *out = cJSON_Print(root);
    if(send(sock_fd , out , 1024 ,0) < 0 ){
        perror("send: 请求服务器失败");
        return 0;
    }
    free(out);
    //printf("%s\n",massage);
    My_Lock();
    /*
    printf("登录上锁前\n");
    pthread_mutex_lock(&mutex);
    printf("登录上锁后\n");
    pthread_cond_wait(&cond ,&mutex);
    printf("登录条件变量为真\n");
    */
    cJSON_Delete(root);
    root = cJSON_Parse(massage);
    item = cJSON_GetObjectItem(root,"res");
    int res = item -> valueint;
    if(res == 1) {
        item = cJSON_GetObjectItem(root,"uid");
        rtn = item -> valueint;
        printf("登录成功!请稍候..");
        fflush(stdout);
        sleep(2);
    }else{
        item = cJSON_GetObjectItem(root,"reason");
        rtn = 0;
        printf("登录失败: %s",item -> valuestring);
        getchar();
    }
    cJSON_Delete(root);
    /*printf("登录解锁前\n");
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
    printf("登录解锁后\n");*/
    My_Unlock();
    return rtn;
}

