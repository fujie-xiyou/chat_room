/*************************************************************************
	>    File Name: Account_Persist.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月08日 星期二 15时42分05秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include "Account_Persist.h"
#include "MySQL.h"
extern MYSQL * mysql;
extern MYSQL_RES * res;
extern MYSQL_ROW row;
char SQL[50];


int Account_Perst_IsUserName(const char * name){
    sprintf(SQL,"SELECT uid FROM account WHERE name = '%s'",name);
    if(mysql_real_query(mysql , SQL , strlen(SQL))){
        return 0;
    }
    res = mysql_store_result(mysql);
    row = mysql_fetch_row(res);
    return atoi(row[0]);
}

int Account_Perst_AddUser(const char *name , const char *password){
    sprintf(SQL,"INSERT INTO account VALUES (NULL , '%s' , md5('%s'))", name , password);
    if(mysql_real_query(mysql , SQL , strlen(SQL))){
        return 0;
    }
    return 1;

}


int Account_Perst_MatchUserAndPassword(int uid , const char * password){
    sprintf(SQL,"SELECT * FROM account WHERE uid = '%d'" 
            "AND password = md5('%s')" , uid , password);
    if(mysql_real_query(mysql , SQL ,strlen(SQL))){
        return 0;
    }
    res = mysql_store_result(mysql);
    if(res) return 1;
    else return 0;

}
