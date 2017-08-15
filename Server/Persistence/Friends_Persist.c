/*************************************************************************
	>    File Name: Friends_Persist.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月14日 星期一 08时20分37秒
 ************************************************************************/

#include<stdio.h>
#include"Friends_Persist.h"
#include"../Service/Friends_Srv.h"
extern MYSQL* mysql;


int Friends_Perst_Add(int uid ,int fuid){
   return 0; 
}

int Friends_Perst_GetList(friends_t * FriendsList ,int uid){
    MYSQL_RES * res , *_res;
    MYSQL_ROW row ,_row;
    char SQL[100];
    friends_t *NewNode = NULL;
    sprintf(SQL ,"SELECT * FROM friends WHERE uid = '%d' AND state = '同意'" ,uid);
    mysql_real_query(mysql ,SQL ,strlen(SQL));
    res = mysql_store_result(mysql);
    while((row = mysql_fetch_row(res))){
        NewNode = (friends_t *)malloc(sizeof(friends_t));
        NewNode -> uid = atoi(row[1]);
        NewNode -> is_follow = atoi(row[2]);
        sprintf(SQL ,"SELECT * FROM account WHERE uid = '%d'" ,NewNode -> uid);
        mysql_real_query(mysql ,SQL,strlen(SQL));
        _res = mysql_store_result(mysql);
        _row = mysql_fetch_row(_res);
        strcpy(NewNode -> name ,_row[1]);
        NewNode -> sex = atoi(_row[2]);
        NewNode -> is_vip = atoi(_row[3]);
        NewNode -> is_online = atoi(_row[4]);
        List_AddHead(FriendsList ,NewNode);
        mysql_free_result(_res);
    }
    mysql_free_result(res);
    return 1;
}

