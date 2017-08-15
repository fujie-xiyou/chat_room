/*************************************************************************
	>    File Name: Account_Srv.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月10日 星期四 11时13分44秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "Account_Srv.h"
#include "List.h"
#include "../Persistence/Account_Persist.h"

extern online_t * OnlineList;


int Account_Srv_ChIsOnline(int uid ,int is_online ,int sock_fd){
    online_t *curPos;
    int rtn = 0;
    if(is_online){
        curPos = (online_t *)malloc(sizeof(online_t));
        curPos -> uid = uid;
        curPos -> sock_fd = sock_fd;
        List_AddHead(OnlineList ,curPos);
        rtn = 1;
    }else{
        List_ForEach(OnlineList ,curPos){
            if(curPos -> uid == uid){
                List_FreeNode(curPos ,online_t);
                rtn = 1;
                break;
            }
        }
    }
    if(Account_Perst_ChIsOnline(uid ,is_online) == 0) rtn = 0;
    return rtn;

}

int Account_Srv_Out(int sock_fd ,char *JSON){
    int uid ,is_online = 0;
    int rtn;
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root ,"uid");
    uid = item -> valueint;
    cJSON_Delete(root);
    rtn = Account_Srv_ChIsOnline(uid ,is_online ,0);
    root = cJSON_CreateObject();
    item = cJSON_CreateString("R");
    cJSON_AddItemToObject(root ,"type" ,item);
    item = cJSON_CreateBool(rtn);
    cJSON_AddItemToObject(root ,"res",item);
    item = cJSON_CreateString("服务器异常");
    cJSON_AddItemToObject(root , "reason" ,item);
    char *out = cJSON_Print(root);
    if(send(sock_fd ,(void*)out , strlen(out) + 1 ,0) <= 0){
        //perror("send ")
        rtn = 0;
    }
    rtn = 1;
    cJSON_Delete(root);
    free(out);
    return rtn;
}


int Account_Srv_SignIn(int sock_fd ,char * JSON){
    char name[30] , password[30];
    int sex;
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root , "name");
    strcpy(name,item -> valuestring);
    item = cJSON_GetObjectItem(root ,"sex");
    sex = item -> valueint;
    item = cJSON_GetObjectItem(root , "password");
    strcpy(password , item -> valuestring);
    cJSON_Delete(root);
    root = cJSON_CreateObject();
    item = cJSON_CreateString("R");
    cJSON_AddItemToObject(root ,"type" ,item);
    if(Account_Perst_IsUserName(name)){
        item = cJSON_CreateBool(0);
        cJSON_AddItemToObject(root , "res" , item);
        item = cJSON_CreateString("用户名已存在");
        cJSON_AddItemToObject(root , "reason" , item);
        char *out = cJSON_Print(root);
        if(send(sock_fd , (void *)out , strlen(out)+1 , 0) < 0){
            //出错 日志处理
        }
        cJSON_Delete(root);
        free(out);
    }else{
        if(Account_Perst_AddUser(name ,sex ,password)){
            item = cJSON_CreateBool(1);
            cJSON_AddItemToObject(root , "res" , item);
            char *out = cJSON_Print(root);
            if(send(sock_fd , (void *)out , strlen(out)+1 , 0) < 0){
                //出错,记录日志
            }
            cJSON_Delete(root);
            free(out);
            return 1;
        }
        item = cJSON_CreateBool(0);
        cJSON_AddItemToObject(root , "res" , item);
        item = cJSON_CreateString("服务器异常");
        cJSON_AddItemToObject(root , "reason" , item);
        char *out = cJSON_Print(root);
        if(send(sock_fd , (void *)out , strlen(out)+1 , 0) < 0){
            //出错 日志处理
        }
        cJSON_Delete(root);
        free(out);
    }
    return 0;
}


int Account_Srv_Login(int sock_fd ,char *JSON){
    char name[30] , password[30];
    int uid;
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root , "name");
    strcpy(name,item -> valuestring);
    item = cJSON_GetObjectItem(root , "password");
    strcpy(password , item -> valuestring);
    cJSON_Delete(root);
    root = cJSON_CreateObject();
    item = cJSON_CreateString("R");
    cJSON_AddItemToObject(root ,"type" ,item);
    if((uid = Account_Perst_IsUserName(name)) == 0){
        item = cJSON_CreateBool(0);
        cJSON_AddItemToObject(root , "res" , item);
        item = cJSON_CreateString("用户名不存在");
        cJSON_AddItemToObject(root , "reason" , item);
        char *out = cJSON_Print(root);
        if(send(sock_fd , (void *)out , strlen(out)+1 , 0) < 0){
            //出错 日志处理
        }
        cJSON_Delete(root);
        free(out);
    }else{
        //用户名存在的
        if(Account_Perst_MatchUserAndPassword(uid ,password)){
            //密码对的
            Account_Srv_ChIsOnline(uid ,1 ,sock_fd);
            item = cJSON_CreateBool(1);
            cJSON_AddItemToObject(root , "res" , item);
            item = cJSON_CreateNumber(uid);
            cJSON_AddItemToObject(root , "uid" ,item);
            char *out = cJSON_Print(root);
            if(send(sock_fd , (void *)out , strlen(out)+1 , 0) < 0){
                //出错,记录日志
            }
            free(out);
            return 1;
        }
        //密码错的
        item = cJSON_CreateBool(0);
        cJSON_AddItemToObject(root , "res" , item);
        item = cJSON_CreateString("用户名密码不匹配");
        cJSON_AddItemToObject(root , "reason" , item);
        char *out = cJSON_Print(root);
        if(send(sock_fd , (void *)out , strlen(out)+1 , 0) < 0){
            //出错 日志处理
        }
        cJSON_Delete(root);
        free(out);
    }
    return 0;
}
