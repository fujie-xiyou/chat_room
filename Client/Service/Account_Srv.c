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
extern int sock_fd;
int Account_Srv_SignIn(const char * name ,const char * password){
    char buf[1024];
    cJSON *root = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString("S");
    cJSON_AddItemToObject(root,"type",item);
    cJSON *msg = cJSON_CreateObject();
    item = cJSON_CreateString(name);
    cJSON_AddItemToObject(msg,"name",item);
    item = cJSON_CreateString(password);
    cJSON_AddItemToObject(msg,"password",item);
    char *out = cJSON_Print(msg);
    item = cJSON_CreateString(out);
    cJSON_AddItemToObject(root,"msg",item);
    free(out);
    out = cJSON_Print(root);
    if(send(sock_fd , (void *)out , strlen(out) + 1 ,0) < 0 ){
        perror("send: 请求服务器失败");
        exit(0);
    }
    cJSON_Delete(msg);
    cJSON_Delete(root);
    if(recv(sock_fd , (void *)buf , sizeof(buf) , 0) < 0){
        perror("recv: 接收服务器响应失败");
        exit(0);
    }
    root = cJSON_Parse(buf);
    item = cJSON_GetObjectItem(root,"res");
    int res = item -> valueint;
    if(res == 1) {
        printf("注册成功!按任意键继续");
        getchar();
        return 1;
    }else{
        item = cJSON_GetObjectItem(root,"reason");
        printf("注册失败: %s",item -> valuestring);
        getchar();
        return 0;
    }
    cJSON_Delete(root);


    
}
