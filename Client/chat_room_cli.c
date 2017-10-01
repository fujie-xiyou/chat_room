/*************************************************************************
	>    File Name: char_room_cli.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月11日 星期五 08时58分20秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include "Common/cJSON.h"
#include "./View/Main_UI.h"
#include "./Service/Connect.h"
extern int sock_fd;
int gl_uid;//记录登录用户的uid
int main(){
    char buf[1024];
    char host[50];
    int fd = open("config.json" ,O_RDONLY);
    if(fd == -1) {
        printf("配置文件打开失败!");
        getchar();
        exit(0);
    }
    read(fd ,buf ,1024);
    cJSON* root = cJSON_Parse(buf);
    cJSON* item = cJSON_GetObjectItem(root ,"host");
    strcpy(host ,item -> valuestring);
    item = cJSON_GetObjectItem(root ,"port");
    int port = item -> valueint;
    close(fd);
    cJSON_Delete(root);
    Connect(host ,port);
    Main_UI_Hello();
    close(sock_fd);
}
