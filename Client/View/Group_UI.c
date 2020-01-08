/*************************************************************************
	>    File Name: Group_UI.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年09月08日 星期五 12时43分52秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include "./Group_UI.h"
#include "../Common/Common.h"
#include "../Common/List.h"
#include "../Service/Group_Srv.h"
#include "../Service/Friends_Srv.h"

extern group_t *GroupList;
extern friends_t *FriendsList;
void Group_UI_Create(){
    char gname[30];
    printf("请输入要创建的群名称:");
    sgets(gname,30);
    Group_Srv_Create(gname);
}
void Group_UI_ShowList(){
    static int g_num = 0;
    printf("▶ 我的群聊(%d)\n" ,g_num);
    g_num = 0;
    group_t * g;
    List_ForEach(GroupList ,g){
        printf("   %s",g -> name);
        if(g -> NewMsgNum > 0) printf(" (\e[31m%d\e[0m)",g -> NewMsgNum);
        printf("\n");
        g_num ++;
    }
}

void Group_UI_AddMember(int gid){
    char name[30]; 
    printf("请输入要邀请的好友名称:");
    scanf("%30s",name);
    ffflush();
    friends_t *f;
    List_ForEach(FriendsList ,f){
        if(strcmp(f -> name ,name) == 0) break;
    }
    if(f == NULL){
        printf("%s 不是你的好友，无法邀请" ,name);
        getchar();
        return;
    }
    Group_Srv_AddMember(gid ,f -> uid);
}
