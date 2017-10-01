/*************************************************************************
	>    File Name: config.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年09月21日 星期四 17时10分01秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include"./cJSON.h"
int main(){
    cJSON *root = cJSON_CreateObject();

    cJSON_AddStringToObject(root ,"host" ,"127.0.0.1");
    cJSON_AddNumberToObject(root ,"port" ,1314);
    char *out = cJSON_Print(root);
    cJSON_Delete(root);
    int fd = open("config.json" ,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
    if(fd == -1){
        perror("open");
        return 0;
    }
    write(fd ,out ,strlen(out));
    close(fd);
    free(out);
}
