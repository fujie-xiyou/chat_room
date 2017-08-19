/*************************************************************************
	>    File Name: Group_Persist.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月18日 星期五 01时34分52秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include"./MySQL.h"
#include"../Service/Group_Srv.h"
#include"../Common/List.h"
#include"../Service/Group_Srv.h"
extern MYSQL *mysql;
int Group_Perst_Create(int uid ,const char *name){
    char SQL[100];
    sprintf(SQL ,"INSERT INTO groups VALUES(NULL ,'%s')", name);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL))){
        printf("%s\n" ,mysql_error(mysql));
        return 0;
    }
    sprintf(SQL ,"INSERT INTO group_member VALUES(LAST_INSERT_ID() ,'%d','0' ,'1')",uid);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL))){
        printf("%s\n" ,mysql_error(mysql));
        return 0;
    }

    return 1;
}

int Group_Perst_AddMember(int gid ,int uid){
    char SQL[100]; 
    sprintf(SQL ,"INSERT INTO group_member VALUES('%d' ,'%d','0' ,'0')",gid,uid);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL))){
        printf("%s\n" ,mysql_error(mysql));
        return 0;
    }

    return 1;
}

int Group_Perst_DeleteMember(int gid ,int uid){
    char SQL[100];
    sprintf(SQL ,"DELETE FROM group_member WHERE (gid = '%d' ADN uid ='%d')", gid ,uid);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL))){
        printf("%s",mysql_error(mysql));
        return 0;
    }
    return 1;
}

int Group_Perst_Delete(int gid){
    char SQL[100];
    sprintf(SQL ,"DELETE FROM groups WHERE gid = '%d'" ,gid);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL))){
        printf("%s",mysql_error(mysql));
        return 0;
    }
    return 1;
}

int Group_Perst_GetMyGroup(group_t *MyGroupList ,int uid){
    group_t *NewNode;
    char SQL[100];
    MYSQL_RES *res ,*_res;
    MYSQL_ROW row ,_row;
    sprintf(SQL ,"SELECT gid FROM group_member WHERE uid = '%d'",uid);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL))){
        printf("%s",mysql_error(mysql));
        return 0;
    }
    res = mysql_store_result(mysql);
    while((row = mysql_fetch_row(res))){
        NewNode = (group_t *)malloc(sizeof(group_t));
        NewNode -> gid = atoi(row[0]);
        sprintf(SQL,"SELECT * FROM groups WHERE gid = '%d'",NewNode -> gid);
        if(mysql_real_query(mysql ,SQL ,strlen(SQL))){
            printf("%s",mysql_error(mysql));
            return 0;
        }
        _res = mysql_store_result(mysql);
        _row = mysql_fetch_row(res);
        strcpy(NewNode -> name ,_row[1]);
        List_AddHead(MyGroupList ,NewNode);
        mysql_free_result(_res);
    }
    mysql_free_result(res);
    return 1;
}


int Group_Perst_GetGroupMember(group_member_t* GroupMember,int gid){
    MYSQL_RES *res,*_res;
    MYSQL_ROW row ,_row;
    char SQL[100];
    group_member_t *NewNode;
    sprintf(SQL ,"SELECT * FROM group_member WHERE gid = '%d'" ,gid);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL))){
        printf("%s",mysql_error(mysql));
        return 0;
    }
    res = mysql_store_result(mysql);
    while((row = mysql_fetch_row(res))){
        sprintf(SQL ,"SELECT name FROM account WHERE (uid = '%s' ADN state = '1'" ,row[1]);
        _res = mysql_store_result(mysql);
        _row = mysql_fetch_row(_res);
        NewNode = (group_member_t *)malloc(sizeof(group_member_t));
        NewNode -> gid = atoi(row[0]);
        NewNode -> uid = atoi(row[1]);
        strcpy(NewNode -> name ,_row[0]);
        NewNode -> is_admin = atoi(row[2]);
        NewNode -> is_own = atoi(row[3]);
        List_AddHead(GroupMember ,NewNode);
        mysql_free_result(_res);
    }   
    mysql_free_result(res);

}
