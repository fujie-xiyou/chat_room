/*************************************************************************
	>    File Name: Chat_UI.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月16日 星期三 08时05分47秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include"../Common/Common.h"
#include"../Common/List.h"
#include"../Service/Friends_Srv.h"
#include"../Service/Chat_Srv.h"
extern int gl_uid;
extern friends_t * FriendsList;
extern private_msg_t* PriMsgList;
void Chat_UI_Private(){
    char msg[1000];
    char fname[30];
    int fuid = 0;
    friends_t * f;
    while(1){
        printf("请输入好友用户名:");
        scanf("%s",fname);
        ffflush();
        List_ForEach(FriendsList ,f){
            //printf("f->name = %s\n" ,f->name);
            if(strcmp(f->name ,fname) == 0){
                fuid = f->uid;
            }
        }
        if(fuid == 0 ){
            printf("%s 不是你的好友,\n"
                   "或者刷新好友列表试试?\n" , fname);
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
            if(m -> from_uid == fuid || m -> from_uid == gl_uid){
                this_msg_num ++;
            }
        }

        List_ForEach(PriMsgList ,m){
            if( m-> from_uid == fuid ){
                if(this_msg_num > 10){
                    List_FreeNode(m ,private_msg_t);
                    this_msg_num --;
                }else{
                    printf("\t\e[31m%s\e[0m ",m->time);
                    printf("%s\n",m -> name);
                    printf("\t  \e[1m%s\e[0m\n",m -> msg);

                }
            }else if(m -> from_uid == gl_uid){
                if(this_msg_num > 10){
                    List_FreeNode(m ,private_msg_t);
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
            List_ForEach(FriendsList ,f){
                if(f -> uid == fuid)
                    f -> NewMsgNum = 0;
            }
            return;   
        }else if(strcmp(msg,"/f") == 0){
            char filename[100];
            while(1){
                printf("请输入文件名:");
                sgets(filename ,100);
                if(*filename == '\0') {
                    break;
                }
                if(Chat_Srv_SendFile(filename ,fuid)){
                   printf("文件发送成功");
                }
                getchar();
            }
        }else{
            Chat_Srv_SendPrivate(fuid ,msg);
        }
    }while(1);
}
