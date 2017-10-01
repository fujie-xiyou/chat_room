/*************************************************************************
	>    File Name: MySQL.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月09日 星期三 09时09分18秒
 ************************************************************************/

#include <stdio.h>
#include "MySQL.h"

MYSQL * mysql;

int MySQL_Connect(const char *host ,const char *user ,const char *pass ,const char *database){
    mysql = mysql_init(NULL);
    if(mysql_real_connect(mysql,host,user,pass,database,0,NULL,0) == 0){
        return 0;
    }
    return 1;
}


void MySQL_Close(){
    mysql_close(mysql);
}
