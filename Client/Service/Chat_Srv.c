/*************************************************************************
	>    File Name: Chat_Srv.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月16日 星期三 08时26分55秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<libgen.h>
#include"./Connect.h"
#include"./Chat_Srv.h"
#include"../Common/cJSON.h"
#include"../Common/List.h"
#include"../Service/Friends_Srv.h"
#include"../Service/Group_Srv.h"
#define MSG_LEN 1024
extern int gl_uid;
extern int sock_fd;
extern friends_t *FriendsList;
extern group_t *GroupList;
private_msg_t *PriMsgList;
group_msg_t *GroMsgList;
int Chat_Srv_RecvFile(const char *JSON){
    char code_out[650] ,buf[900];
    memset(buf ,0,sizeof(buf));
    char filename [100] = "RecvFile/";
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root ,"filename");
    strcat(filename , item->valuestring);
    int fd = open(filename ,O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR);
    if(fd == -1){
        perror("open");
        return 0;
    }
    item = cJSON_GetObjectItem(root ,"con");
    strcat( buf , item -> valuestring);
    item = cJSON_GetObjectItem(root ,"size");
    int size = item -> valueint;
    base64_decodestate state_in;
    base64_init_decodestate(&state_in);
    base64_decode_block(buf ,strlen(buf) ,code_out ,&state_in);
    if(write(fd,code_out ,size) != size){
        perror("write");
        close(fd);
        return 0;
    }
    if(size < 650 - 2) {
        item = cJSON_GetObjectItem(root ,"uid");
        int uid = item -> valueint;
        friends_t *f;
        List_ForEach(FriendsList ,f){
            if(f -> uid == uid){
                printf("\n%s 发来一个文件,已保存至./RecvFile/%s\n",
                        f->name ,basename(filename));
                break;
            }
        }
    }
    close(fd);
    return 0;
}
int Chat_Srv_SendFile(const char *filename ,int fuid){
    char buf[650] ,code_out[900] ,code_end[5],*out;
    int fd ,size;
    base64_encodestate state_in;
    if((fd = open(filename ,O_RDONLY)) == -1){
        printf("文件不存在或无读取权限");
        return 0;
    }
    while(1){
        memset(buf ,0,sizeof(buf));
        size = read(fd ,buf ,sizeof(buf) - 2);
        //650 - 2 刚好是24的整数倍 转码清晰
        base64_init_encodestate(&state_in);
        memset(code_out ,0,sizeof(code_out));
        base64_encode_block(buf ,size ,code_out ,&state_in);
        if(state_in.step != step_A) {
            memset(code_end ,0,sizeof(code_end));
            base64_encode_blockend(code_end ,&state_in);
            strcat(code_out ,code_end);
        }
        //step_A代表刚好转成base64时不需要补位
        cJSON *root = cJSON_CreateObject();
        cJSON *item = cJSON_CreateString("F");
        cJSON_AddItemToObject(root ,"type" ,item);
        item = cJSON_CreateNumber(gl_uid);
        cJSON_AddItemToObject(root ,"uid",item);
        item = cJSON_CreateNumber(fuid);
        cJSON_AddItemToObject(root ,"fuid" ,item);
        item = cJSON_CreateString(basename((char*)filename));
        cJSON_AddItemToObject(root ,"filename" ,item);
        item = cJSON_CreateNumber(size);
        cJSON_AddItemToObject(root ,"size" ,item);
        item = cJSON_CreateString(code_out);
        cJSON_AddItemToObject(root ,"con",item);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        int ret;
	printf("%s\n",out);
        if((ret = send(sock_fd ,out,MSG_LEN ,0)) <= 0){
            perror("send");
            free(out);
            return 0;
        }
        if(size < (int)sizeof(buf) - 2) break;
        free(out);
    }
    close(fd);
    return 1;
}
void Chat_Srv_InitList(){
    List_Init(PriMsgList ,private_msg_t);
    List_Init(GroMsgList ,group_msg_t);
}
void Chat_Srv_RecvPrivate(const char *JSON){
    private_msg_t *NewMsg = (private_msg_t *)malloc(sizeof(private_msg_t));
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root ,"from_uid");
    NewMsg->from_uid = item -> valueint;
    item = cJSON_GetObjectItem(root ,"msg");
    strcpy(NewMsg->msg , item -> valuestring);
    item = cJSON_GetObjectItem(root ,"time");
    strcpy(NewMsg->time , item -> valuestring);
    friends_t * f;
    List_ForEach(FriendsList ,f){
        if(f->uid == NewMsg->from_uid){
            strcpy(NewMsg->name ,f->name);
            (f->NewMsgNum) ++;
        }
    }
    NewMsg->next =NULL;
    cJSON_Delete(root);
    private_msg_t *curPos;
    List_AddTail(PriMsgList ,curPos ,NewMsg);
    printf("\n%s 发来一条消息\n",NewMsg->name);
}

void Chat_Srv_RecvGroup(const char *JSON){
    group_msg_t *NewMsg = 
        (group_msg_t *)malloc(sizeof(group_msg_t));
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root ,"from_uid");
    NewMsg->from_uid = item -> valueint;
    item = cJSON_GetObjectItem(root ,"to_gid");
    NewMsg -> gid = item -> valueint;
    item = cJSON_GetObjectItem(root ,"msg");
    strcpy(NewMsg->msg , item -> valuestring);
    item = cJSON_GetObjectItem(root ,"time");
    strcpy(NewMsg->time , item -> valuestring);
    item = cJSON_GetObjectItem(root,"uname");
    strcpy(NewMsg -> uname ,item -> valuestring);
    group_t * g;
    List_ForEach(GroupList ,g){
        if(g->gid == NewMsg->gid){
            strcpy(NewMsg->gname ,g->name);
            (g->NewMsgNum) ++;
        }
    }
    NewMsg->next =NULL;
    cJSON_Delete(root);
    group_msg_t *curPos;
    List_AddTail(GroMsgList ,curPos ,NewMsg);
    printf("\n群聊 %s 有一条新消息\n",NewMsg->gname);
}

int Chat_Srv_SendPrivate(int to_uid ,const char * msg){
    int rtn = 1;
    private_msg_t *NewMsg = 
        (private_msg_t *)malloc(sizeof(private_msg_t));
    user_date_t Srvdate = DateNow();
    user_time_t Srvtime = TimeNow();
    char Srvdatetime[25];
    sprintf(Srvdatetime ,"%04d-%02d-%02d %02d:%02d:%02d",
            Srvdate.year ,Srvdate.month ,Srvdate.day,
            Srvtime.hour ,Srvtime.minute ,Srvtime.second);
    strcpy(NewMsg->time ,Srvdatetime);
    NewMsg->from_uid = gl_uid;
    strcpy(NewMsg->msg ,msg);
    NewMsg -> next = NULL;
    private_msg_t *m;
    List_AddTail(PriMsgList ,m ,NewMsg);    
    cJSON *root = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString("P");
    cJSON_AddItemToObject(root ,"type" ,item);
    item = cJSON_CreateNumber(gl_uid);
    cJSON_AddItemToObject(root ,"from_uid" ,item);
    item = cJSON_CreateNumber(to_uid);
    cJSON_AddItemToObject(root ,"to_uid" ,item);
    item = cJSON_CreateString(msg);
    cJSON_AddItemToObject(root ,"msg" ,item);
    char *out = cJSON_Print(root);
    if(send(sock_fd ,(void *)out ,MSG_LEN ,0) <= 0){
        printf("服务器失去响应\n");
        rtn = 0;
    }
    cJSON_Delete(root);
    free(out);
    return rtn;
}
int Chat_Srv_SendGroup(int to_gid ,const char * msg){
    int rtn = 1;
    group_msg_t *NewMsg = 
        (group_msg_t *)malloc(sizeof(group_msg_t));
    NewMsg -> gid = to_gid;
    user_date_t Srvdate = DateNow();
    user_time_t Srvtime = TimeNow();
    char Srvdatetime[25];
    sprintf(Srvdatetime ,"%04d-%02d-%02d %02d:%02d:%02d",
            Srvdate.year ,Srvdate.month ,Srvdate.day,
            Srvtime.hour ,Srvtime.minute ,Srvtime.second);
    strcpy(NewMsg->time ,Srvdatetime);
    NewMsg->from_uid = gl_uid;
    strcpy(NewMsg->msg ,msg);
    NewMsg -> next = NULL;
    group_msg_t *m;
    List_AddTail(GroMsgList ,m ,NewMsg);    
    cJSON *root = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString("p");
    cJSON_AddItemToObject(root ,"type" ,item);
    item = cJSON_CreateNumber(gl_uid);
    cJSON_AddItemToObject(root ,"from_uid" ,item);
    item = cJSON_CreateNumber(to_gid);
    cJSON_AddItemToObject(root ,"to_gid" ,item);
    item = cJSON_CreateString(msg);
    cJSON_AddItemToObject(root ,"msg" ,item);
    char *out = cJSON_Print(root);
    if(send(sock_fd ,(void *)out ,MSG_LEN ,0) <= 0){
        printf("服务器失去响应\n");
        rtn = 0;
    }
    cJSON_Delete(root);
    free(out);
    return rtn;
}
void Chat_Srv_ShowPrivateRec(const char *JSON){
    private_msg_t *NewMsg = (private_msg_t *)malloc(sizeof(private_msg_t));
    cJSON *root = cJSON_Parse(JSON);
    cJSON *item = cJSON_GetObjectItem(root ,"from_uid");
    NewMsg->from_uid = item -> valueint;
    item = cJSON_GetObjectItem(root ,"msg");
    strcpy(NewMsg->msg , item -> valuestring);
    item = cJSON_GetObjectItem(root ,"time");
    strcpy(NewMsg->time , item -> valuestring);
    friends_t * f;
    List_ForEach(FriendsList ,f){
        if(f->uid == NewMsg->from_uid){
            strcpy(NewMsg->name ,f->name);
            (f->NewMsgNum) ++;
        }
    }
    NewMsg->next =NULL;
    cJSON_Delete(root);
    if(NewMsg -> from_uid == gl_uid){
        printf("\t\e[32m%s\e[0m ",NewMsg->time);
        printf("我\n");
        printf("\t  \e[1m%s\e[0m\n",NewMsg -> msg);
    }
    else{
        printf("\t\e[31m%s\e[0m ",NewMsg->time);
        printf("%s\n",NewMsg -> name);
        printf("\t  \e[1m%s\e[0m\n",NewMsg -> msg);
    }
}
void Chat_Srv_GetPrivateRec(int fuid){
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root ,"type" ,"E");
    cJSON_AddNumberToObject(root ,"uid" ,gl_uid);
    cJSON_AddNumberToObject(root ,"fuid" ,fuid);
    char *out = cJSON_Print(root);
    cJSON_Delete(root);
    if(send(sock_fd ,(void *)out ,MSG_LEN ,0) <= 0){
        printf("服务器失去响应\n");
    }
    free(out);
    system("clear");
    sleep(1);
}
