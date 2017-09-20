/*************************************************************************
	>    File Name: Friends_Persist.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月14日 星期一 08时20分37秒
 ************************************************************************/

#include<stdio.h>
#include"Friends_Persist.h"
#include"../Service/Friends_Srv.h"
#include"../Common/List.h"
extern MYSQL* mysql;


int Friends_Perst_Add(int uid ,int fuid){
    char SQL[100];
    sprintf(SQL ,"INSERT INTO friends VALUES('%d' ,'%d' ,'0' ,'0')",uid ,fuid);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL))){
        printf("%s",mysql_error(mysql));
        return 0;
    }
   return 1; 
}

int Friends_Perst_Apply(int uid ,int fuid ,int is_agree){
    char SQL[100];
    if(is_agree)
        sprintf(SQL ,"UPDATE friends SET state  = '1' WHERE (uid ='%d' AND fuid = '%d')",uid ,fuid);
    else 
        sprintf(SQL ,"DELETE FROM friends WHERE uid = '%d' AND fuid = '%d'",uid ,fuid);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL))){
        printf("%s",mysql_error(mysql));
        return 0;
    }
    return 1;
}

int Friends_Perst_GetList(friends_t * FriendsList ,int uid){
    MYSQL_RES * res , *_res;
    MYSQL_ROW row ,_row;
    char SQL[100];
    friends_t *NewNode = NULL;
    sprintf(SQL ,
            "SELECT * FROM friends WHERE (uid = '%d' OR fuid = '%d')" ,
            uid ,uid);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL))){
        printf("%s",mysql_error(mysql));
        return 0;
    }
    res = mysql_store_result(mysql);
    while((row = mysql_fetch_row(res))){
        NewNode = (friends_t *)malloc(sizeof(friends_t));
        NewNode -> uid = atoi(row[(uid != atoi(row[1]))]);
        NewNode -> is_follow = atoi(row[2]);
        NewNode -> state = atoi(row[3]);
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
int Friends_Perst_GetFriendInfo(friends_t *Node){
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
    Node -> is_online = atoi(row[4]);
    Node -> is_follow = 0;
    Node -> state = 0;
    mysql_free_result(res);
    return 1;
}


