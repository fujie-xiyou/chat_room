/*************************************************************************
	>    File Name: Friends_UI.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月15日 星期二 16时00分12秒
 ************************************************************************/

#include <stdio.h>
#include "Friends_UI.h"
#include "../Service/Friends_Srv.h"
#include "../Service/List.h"

extern friends_t *FriendsList;

void Friends_UI_ShowList(){
    friends_t *curPos = NULL;
    printf("▶ 我的好友(%d/%d)\n",3,6);
    char *is_online[2] = {"●","\e[1;32m●\e[0m"};
    char *is_vip[2] = {"","\e[31m"};
    char *sex[2] = {"\e[35m♀\e[0m","\e[36m♂\e[0m"};
    char *is_follow[2] = {"","\e[31m♥\e[0m"};
    List_ForEach(FriendsList ,curPos){
        printf("   %s %s%s\e[0m %s %s\n" ,
                is_online[curPos->is_online] ,
                is_vip[curPos->is_vip],
                curPos -> name ,sex[curPos->sex] ,
                is_follow[curPos->is_follow]);
    }
}
