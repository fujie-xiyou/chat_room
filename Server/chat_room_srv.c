/*************************************************************************
	>    File Name: char_room_srv.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月10日 星期四 15时17分19秒
 ************************************************************************/

#include <stdio.h>
#include "Service/Connect.h"
#include "Persistence/MySQL.h"
int main(){
    if(MySQL_Connect() == 0){
        printf("数据库连接失败\n");
        exit(0);
    }
    Connect();
}
