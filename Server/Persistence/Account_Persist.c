/*************************************************************************
	>    File Name: Account_Persist.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月08日 星期二 15时42分05秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Account_Persist.h"
#include "MySQL.h"
extern MYSQL * mysql;

int Account_Perst_ChIsOnline(int uid ,int is_online){
    char SQL[100];
    sprintf(SQL ,"UPDATE account SET is_online = '%d' WHERE uid = '%d'" ,is_online ,uid);
    if(mysql_real_query(mysql , SQL , strlen(SQL))){
        printf("%s\n",mysql_error(mysql));
        return 0;
    }
    return 1;
    
}


int Account_Perst_IsUserName(const char * name){
    char SQL[100];
    MYSQL_RES * res;
    MYSQL_ROW row;
    int rtn = 0;
    sprintf(SQL,"SELECT uid FROM account WHERE name = '%s'",name);
    if(mysql_real_query(mysql , SQL , strlen(SQL))){
        return 0;
    }
    res = mysql_store_result(mysql);
    row = mysql_fetch_row(res);
    if(row) rtn = atoi(row[0]);
    mysql_free_result(res);
    return rtn;
}

int Account_Perst_AddUser(const char *name ,int sex , const char *password){
    char SQL[100];
    sprintf(SQL,"INSERT INTO account VALUES (NULL , '%s' ,'%d' , 0, 0 , md5('%s'))", name ,sex , password);
    if(mysql_real_query(mysql , SQL , strlen(SQL))){
        printf("%s\n",mysql_error(mysql));
        return 0;
    }
    return 1;

}


int Account_Perst_MatchUserAndPassword(int uid , const char * password){
    char SQL[100];
    MYSQL_RES * res;
    MYSQL_ROW row;
    int rtn;
    sprintf(SQL,"SELECT * FROM account WHERE (uid = '%d' AND password = md5('%s'))" 
            , uid , password);
    if(mysql_real_query(mysql , SQL ,strlen(SQL))){
        return 0;
    }
    res = mysql_store_result(mysql);
    row = mysql_fetch_row(res);
    if(row) rtn = 1;
    else rtn = 0;
    mysql_free_result(res);
    return rtn;

}
/*
int Account_Perst_GetUserInfo(friends_t *Node){
    char SQL[100];
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(SQL ,"SELECT * FROM account WHERE uid = '%d'" ,Node -> uid);
    mysql_real_query(mysql ,SQL, strlen(SQL));
    res = mysql_store_result(mysql);
    row = mysql_fetch_row(res);
    strcpy(Node -> name ,row[1]);
    Node -> sex = atoi(row[2]);
    Node -> is_vip = atoi(row[3]);
    Node -> is_online atoi(row[4]);
    mysql_free_result(res);
}
*/
