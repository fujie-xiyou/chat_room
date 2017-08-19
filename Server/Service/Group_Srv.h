/*************************************************************************
	>    File Name: Group_Srv.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月18日 星期五 02时22分37秒
 ************************************************************************/

#ifndef _GROUP_SRV_H
#define _GROUP_SRV_H
typedef struct group_t{
    int gid;
    char name[50];
    struct group_t *next;
}group_t;

typedef struct group_member{
    int gid;
    int uid;
    char name[30];
    int is_admin;
    int is_own;
    struct group_member *next;
}group_member_t;


#endif

