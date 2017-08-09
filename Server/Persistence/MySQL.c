/*************************************************************************
	>    File Name: MySQL.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月09日 星期三 09时09分18秒
 ************************************************************************/

#include<stdio.h>
#include<"MySQL.h">

MYSQL * mysql;
MYSQL_RES * res;
MYSQL_ROW row;

int MySQL_Connect(){
    mysql = mysql_init(NULL);
    if(mysql_real_connect(mysql,"sql.fujie.bid","root","AAA000aaa","chat_room",0,NULL,0) == 0){
        return 0;
    }
    return 1;
}


int MySQL_Close(){
    mysql_free_result(res);
    mysql_close(mysql);
}
