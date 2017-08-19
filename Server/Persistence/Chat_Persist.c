/*************************************************************************
	>    File Name: Chat_Persist.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月16日 星期三 09时56分26秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"./Chat_Persist.h"
#include<mysql/mysql.h>
extern MYSQL *mysql;

int Chat_Perst_Private(int from_uid ,int to_uid ,const char *msg, int is_offline){
    char SQL[100 + strlen(msg)];
    sprintf(SQL ,
            "INSERT INTO private_rec VALUES('%d' ,'%d' ,'%s','%d')" ,
            from_uid ,to_uid ,msg  ,is_offline);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL))){
        printf("%s",mysql_error(mysql));
        return 0;
    }
    return 1;
}
int Chat_Perst_Group(int uid ,int gid ,const char *msg, const char *offlist){
    char SQL[100 + strlen(msg)];
    sprintf(SQL ,
            "INSERT INTO group_rec VALUES('%d' ,'%d' ,'%s','%s')" ,
            gid ,uid ,msg ,offlist);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL))){
        printf("%s",mysql_error(mysql));
        return 0;
    }
    return 1;
}
//int Chat_Perst_GetOffline(ugid)
