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
#include "../Persistence/Account_Persist.h"


int Account_Srv_SignIn(int sock_id , char * JSON){
    char name[20] , password[20];
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root , "name");
    strcpy(name,item -> valuestring);
    item = cJSON_GetObjectItem(root , password);
    strcpy(password , item -> valuestring);
    cJSON_Delete(root);
    root = cJSON_CreateObject();
    if(Account_Perst_IsUserName(name)){
        item = cJSON_CreateBool(0);
        cJSON_AddItemToObject(root , "res" , item);
        item = cJSON_CreateString("用户名已存在");
        cJSON_AddItemToObject(root , "reason" , item);
        char *out = cJSON_Print(root);
        if(send(sock_id , (void *)out , strlen(out)+1 , 0) < 0){
            //出错 日志处理
        }
        cJSON_Delete(root);
        free(out);
    }else{
        if(Account_Perst_AddUser(name ,password)){
            item = cJSON_CreateBool(1);
            cJSON_AddItemToObject(root , "res" , item);
            char *out = cJSON_Print(root);
            if(send(sock_id , (void *)out , strlen(out)+1 , 0) < 0){
                //出错,记录日志
            }
            return 1;
        }
        item = cJSON_CreateBool(0);
        cJSON_AddItemToObject(root , "res" , item);
        item = cJSON_CreateString("服务器异常");
        cJSON_AddItemToObject(root , "reason" , item);
        char *out = cJSON_Print(root);
        if(send(sock_id , (void *)out , strlen(out)+1 , 0) < 0){
            //出错 日志处理
        }
        cJSON_Delete(root);
        free(out);
    }
    return 0;
}


int Account_Srv_Login(int sock_id , char *JSON){
    
}
