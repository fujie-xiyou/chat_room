/*************************************************************************
	>    File Name: Chat_UI.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月16日 星期三 08时05分47秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include"./Group_UI.h"
#include"../Common/Common.h"
#include"../Common/List.h"
#include"../Service/Friends_Srv.h"
#include"../Service/Chat_Srv.h"
#include"../Service/Group_Srv.h"
extern int gl_uid;
extern friends_t * FriendsList;
extern group_t * GroupList;
extern private_msg_t* PriMsgList;
extern group_msg_t* GroMsgList;
group_t *curGroup = NULL;
void Chat_UI_Private(){
    friends_t *curFriend = NULL;
    char msg[1000];
    char fname[30];
    friends_t * f;
    while(1){
        printf("请输入好友用户名:");
        scanf("%30s",fname);
        ffflush();
        List_ForEach(FriendsList ,f){
            //printf("f->name = %s\n" ,f->name);
            if(strcmp(f->name ,fname) == 0){
                curFriend = f;
            }
        }
        if(curFriend == NULL ){
            printf("%s 不是你的好友." , fname);
            getchar();
            return;
        }else{
            break;
        }
    }
    int this_msg_num;
    private_msg_t * m;
    do{
        system("clear");
        printf( "---------------------------------------\n"
                " -                私聊               -\n"
                "---------------------------------------\n");
        this_msg_num = 0;
        List_ForEach(PriMsgList ,m){
            if(m -> from_uid == curFriend -> uid || m -> from_uid == gl_uid){
                this_msg_num ++;
            }
        }

        List_ForEach(PriMsgList ,m){
            if( m-> from_uid == curFriend -> uid ){
                if(this_msg_num > 10){
                    List_FreeNode(PriMsgList ,m ,private_msg_t);
                    this_msg_num --;
                }else{
                    printf("\t\e[31m%s\e[0m ",m->time);
                    printf("%s\n",m -> name);
                    printf("\t  \e[1m%s\e[0m\n",m -> msg);

                }
            }else if(m -> from_uid == gl_uid){
                if(this_msg_num > 10){
                    List_FreeNode(PriMsgList ,m ,private_msg_t);
                    this_msg_num --;
                }else{
                    printf("\t\e[32m%s\e[0m ",m->time);
                    printf("我\n");
                    printf("\t  \e[1m%s\e[0m\n",m -> msg);
                }
            }
        }
        printf( "---------------------------------------\n"
                "功能: /r  :  返回上一级\n"
                "      /f  :  发送文件\n"
                "      /m  :  聊天记录\n"
                "      回车:  发送/刷新消息\n"
                "---------------------------------------\n");
        printf("消息/功能:");
        sgets(msg ,1000);
        if(*msg == '\0') continue;
        else if(strcmp(msg,"/r") == 0) {
            curFriend -> NewMsgNum = 0;
            return;   
        }else if(strcmp(msg,"/f") == 0){
            if(curFriend -> is_online == 0){
                printf("当前好友不在线,无法发送文件\n");
                getchar();
                continue;
            }
            char filename[100];
            while(1){
                printf("请输入文件路径:");
                sgets(filename ,100);
                if(*filename == '\0') {
                    break;
                }
                if(Chat_Srv_SendFile(filename ,curFriend -> uid)){
                   printf("文件发送成功");
                }
                getchar();
                break;
            }
        }
        else if(strcmp(msg,"/m") == 0) {
            Chat_Srv_GetPrivateRec(curFriend -> uid);
            getchar();
        }else{
            Chat_Srv_SendPrivate(curFriend -> uid,msg);
        }
    }while(1);
}

void Chat_UI_Group(){
    group_t *g = NULL;
    char msg[1000];
    char gname[30];
    while(1){
        printf("请输群名称:");
        scanf("%s",gname);
        ffflush();
        List_ForEach(GroupList ,g){
            if(strcmp(g->name ,gname) == 0){
                curGroup = g;
            }
        }
        if(curGroup == NULL ){
            printf("你不在群聊 %s 中" , gname);
            getchar();
            return;
        }else{
            break;
        }
    }
    int this_msg_num;
    group_msg_t * m;
    do{
        if(curGroup == NULL) return ;
        system("clear");
        printf( "---------------------------------------\n"
                " -                群聊               -\n"
                "---------------------------------------\n");
        this_msg_num = 0;
        List_ForEach(GroMsgList ,m){
            if(m -> gid == curGroup -> gid || m -> from_uid == gl_uid){
                this_msg_num ++;
            }
        }

        List_ForEach(GroMsgList ,m){
            if( m-> gid == curGroup -> gid ){
                if(this_msg_num > 10){
                    List_FreeNode(GroMsgList ,m ,group_msg_t);
                    this_msg_num --;
                }else{
                    if(m -> from_uid == gl_uid){
                        printf("\t\e[32m%s\e[0m ",m->time);
                        printf("我\n");
                        printf("\t  \e[1m%s\e[0m\n",m -> msg);
                    }else {
                        printf("\t\e[31m%s\e[0m ",m->time);
                        printf("%s\n",m -> uname);
                        printf("\t  \e[1m%s\e[0m\n",m -> msg);
                    }
                }
            }
        }
        printf( "---------------------------------------\n"
                "/r 返回上级| /m  聊天记录| /l 查看群成员\n"
                "/a 邀请好友| /q 退/解散群| 回车 发送/刷新\n"
                "---------------------------------------\n");
        printf("消息/功能:");
        sgets(msg ,1000);
        if(*msg == '\0') continue;
        else if(strcmp(msg,"/r") == 0) {
            curGroup -> NewMsgNum = 0;
            return;   
        }else if(strcmp(msg ,"/m") == 0){
            //查看聊天记录
        }else if(strcmp(msg ,"/l") == 0){
            system("clear");
            printf("群成员列表(%s):\n", curGroup-> name);
            Group_Srv_GetMember(curGroup -> gid);
            //查看群成员
            sleep(1);
            getchar();
        }else if(strcmp(msg ,"/a") == 0){
            //邀请好友
            Group_UI_AddMember(curGroup -> gid);
        }else if(strcmp(msg ,"/q") == 0){
            //退群或解散群
            Group_Srv_Quit(curGroup);
            curGroup = NULL;
        }
        else{
            Chat_Srv_SendGroup(curGroup -> gid,msg);
        }
    }while(1);
}
