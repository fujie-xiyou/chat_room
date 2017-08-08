/*************************************************************************
	>    File Name: Account_Persist.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月08日 星期二 15时42分05秒
 ************************************************************************/

#include<stdio.h>
#include<"Account_Persist.h">
extern MYSQL * mysql;
extern MYSQL_RES * res;
extern MYSQL_ROW row;
int Account_Perst_IsUserName(const char * name){
    char SQL[50];
    sprintf(SQL,"SELECT uid FROM account WHERE name = '%s'",name);
    if(mysql_real_query(mysql , SQl , strlen(SQL))){
        printf("很抱歉.查询数据库失败:%s\n",mysql_error(mysql));
        exit(0);
    }


}
