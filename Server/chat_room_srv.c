/*************************************************************************
	>    File Name: char_room_srv.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月10日 星期四 15时17分19秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "Common/cJSON.h"
#include "Service/Connect.h"
#include "Persistence/MySQL.h"
int main(){
    char buf[1024];
    char host[50] ,user[30],pass[50],database[50];
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
    item = cJSON_GetObjectItem(root ,"user");
    strcpy(user ,item -> valuestring);
    item = cJSON_GetObjectItem(root ,"pass");
    strcpy(pass ,item -> valuestring);
    item = cJSON_GetObjectItem(root ,"database");
    strcpy(database ,item -> valuestring);
    item = cJSON_GetObjectItem(root ,"port");
    int port = item -> valueint;
    close(fd);
    cJSON_Delete(root);
    if(MySQL_Connect(host ,user ,pass ,database) == 0){
        printf("数据库连接失败\n");
        exit(0);
    }
    Connect(port);
}
