/*************************************************************************
	>    File Name: Friends_UI.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月15日 星期二 16时00分12秒
 ************************************************************************/

#include <stdio.h>
#include "Friends_UI.h"
#include "./Account_UI.h"
#include "../Service/Friends_Srv.h"
#include "../Common/List.h"

extern friends_t *FriendsList;
extern int PriMsgNum;
extern int gl_uid;
int f_num = 0 ,online_num = 0 ,a_num = 0;
void Friends_UI_ShowList(){
    friends_t *curPos = NULL;
    printf("▶ 我的好友(%d/%d)\n",online_num ,f_num);
    f_num = online_num = 0;
    char *is_online[2] = {"●","\e[32m●\e[0m"};
    char *is_vip[2] = {"","\e[31m"};
    char *sex[2] = {"\e[35m♀\e[0m","\e[36m♂\e[0m"};
    char *is_follow[2] = {"","\e[31m♥\e[0m"};
    char is_msg[2][20]={"",""};
    List_ForEach(FriendsList ,curPos){
        if(curPos -> state != 1) {
            a_num ++;
            continue;
        }
        f_num ++;
        if(curPos->is_online) online_num ++;
        sprintf(is_msg[0],"(\e[31m%d\e[0m)" ,curPos -> NewMsgNum);
        printf("   %s %s%s\e[0m %s %s %s\n" ,
            is_online[curPos->is_online] ,
            is_vip[curPos->is_vip],
            curPos -> name ,sex[curPos->sex] ,
            is_follow[curPos->is_follow],
            is_msg[(curPos->NewMsgNum == 0)]);
    }

}
void Friends_UI_ShowApply(){
    //打印群聊列表
    printf("▶ 申请列表(%d)\n" ,a_num);
    a_num = 0;
    friends_t *curPos;
    List_ForEach(FriendsList ,curPos){
        if(curPos -> state != 0 || curPos -> uid == gl_uid) continue;
        printf("   %s 申请加你为好友\n",curPos->name);
    }
}


void Friends_UI_Add(){
    char fname[30];
    printf("请输入待添加的好友名:");
    scanf("%30s",fname);
    ffflush();
    friends_t *f;
    List_ForEach(FriendsList ,f){
        if(strcmp(f->name ,fname) == 0) {
            printf("%s 已经是你的好友了.",fname);
            getchar();
            return;
        }
    }
    Friends_Srv_SendAdd(fname);
}

void Friends_UI_Apply(){
    friends_t *f;
    char choice;
    List_ForEach(FriendsList ,f){
        if(f -> state != 0) continue;
        printf("是否同意 %s 的好友请求?\n请输入:(y/n 其他返回)", f->name);
        scanf("%c",&choice);
        if(choice == 'y'){
            Friends_Srv_Apply(f -> uid ,gl_uid ,1);
            f -> state = 1;
        }else if(choice == 'n'){
            Friends_Srv_Apply(f->uid ,gl_uid ,0);
            List_FreeNode(FriendsList ,f ,friends_t);
        }
    }
}
