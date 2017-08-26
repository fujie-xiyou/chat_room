/*************************************************************************
	>    File Name: Chat_Srv.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月16日 星期三 08时57分17秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include"./Account_Srv.h"
#include"./Connect.h"
#include"./Chat_Srv.h"
#include"./Group_Srv.h"
#include"../Persistence/Chat_Persist.h"
#include"../Persistence/Group_Persist.h"
#include"../Common/Common.h"
#include"../Common/List.h"
#include"../Common/cJSON.h"
#define MSG_LEN 1024

extern online_t *OnlineList;

void Chat_Srv_File(const char *JSON){
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root ,"fuid");
    int fuid = item -> valueint;
    item = cJSON_GetObjectItem(root ,"size");
    printf("size = %d\n" ,item -> valueint);
    item = cJSON_GetObjectItem(root,"con");
    printf("base64 :\n%s\n",item -> valuestring);
    int f_sock_fd = -1;
    online_t *o;
    List_ForEach(OnlineList ,o){
        if(o -> uid == fuid){
            f_sock_fd = o-> sock_fd;
            break;
        }
    }
    if(f_sock_fd == -1) return ;
    if(send(f_sock_fd ,JSON ,MSG_LEN,0) <= 0){
        perror("send");
        cJSON_Delete(root);
        return ;
    }
    cJSON_Delete(root);
    return ;
}

int Chat_Srv_GetFriendSock(int fuid){
    online_t *curPos;
    int to_sock = -1;
    List_ForEach(OnlineList ,curPos){
        if(curPos -> uid == fuid){
            to_sock = curPos -> sock_fd;
        }
    }
    return to_sock;
}
int Chat_Srv_Private(int sock_fd ,const char *JSON){
    int from_uid ,to_uid ,to_sock ;
    user_date_t Srvdate = DateNow();
    user_time_t Srvtime = TimeNow();
    char Srvdatetime[25];
    sprintf(Srvdatetime ,"%04d-%02d-%02d %02d:%02d:%02d",
            Srvdate.year ,Srvdate.month ,Srvdate.day,
            Srvtime.hour ,Srvtime.minute ,Srvtime.second);
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root ,"from_uid");
    from_uid = item -> valueint;
    item = cJSON_GetObjectItem(root ,"to_uid");
    to_uid = item -> valueint;
    item = cJSON_CreateString(Srvdatetime);
    cJSON_AddItemToObject(root ,"time" ,item);
    char *out = cJSON_Print(root);
    cJSON_Delete(root);
    to_sock = Chat_Srv_GetFriendSock(to_uid);
    Chat_Perst_Private(from_uid ,to_uid ,out ,(to_sock == -1));
    if(to_sock == -1) return 2;
    if(send(to_sock ,(void *)out ,MSG_LEN ,0) <= 0){
        perror("send:");
        free(out);
        return 0;
    }
    free(out);
    return 1;
}
int Chat_Srv_Group(int sock_fd ,const char *JSON){
    int uid ,gid ,to_sock ;
    char offlist[100] ,str[4];
    group_member_t *GroupMember ,*g;
    List_Init(GroupMember ,group_member_t);
    user_date_t Srvdate = DateNow();
    user_time_t Srvtime = TimeNow();
    char Srvdatetime[25];
    sprintf(Srvdatetime ,"%04d-%02d-%02d %02d:%02d:%02d",
            Srvdate.year ,Srvdate.month ,Srvdate.day,
            Srvtime.hour ,Srvtime.minute ,Srvtime.second);
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root ,"uid");
    uid = item -> valueint;
    item = cJSON_GetObjectItem(root ,"gid");
    gid = item -> valueint;
    item = cJSON_CreateString(Srvdatetime);
    cJSON_AddItemToObject(root ,"time" ,item);
    char *out = cJSON_Print(root);
    cJSON_Delete(root);
    Group_Perst_GetGroupMember(GroupMember ,gid);
    List_ForEach(GroupMember ,g){
        to_sock = Chat_Srv_GetFriendSock(g->uid);
        if(to_sock == -1) {
            sprintf(str ,"%d,",g->uid);
            strcat(offlist ,str);
            continue;
        }
        if(send(to_sock ,(void *)out ,MSG_LEN ,0) <= 0){
            perror("send:");
            free(out);
            return 0;
        }
    }
    Chat_Perst_Group(uid ,gid ,out ,offlist);
    free(out);
    return 1;
}
