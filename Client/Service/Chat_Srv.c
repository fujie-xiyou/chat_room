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
#include"./Connect.h"
#include"./Chat_Srv.h"
#include"../Common/cJSON.h"
#include"../Common/List.h"
#include"../Service/Friends_Srv.h"
extern int gl_uid;
extern int sock_fd;
extern friends_t *FriendsList;
private_msg_t *PriMsgList;
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
    printf("\n%s\n",buf);
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
    if(size < 650) {
        item = cJSON_GetObjectItem(root ,"uid");
        int uid = item -> valueint;
        friends_t *f;
        List_ForEach(FriendsList ,f){
            if(f -> uid == uid){
                printf("\n%s 发来一个文件,已保存至./RecvFile/%s\n",
                        f->name ,filename);
                break;
            }
        }
    }
    close(fd);
}
int Chat_Srv_SendFile(const char *filename ,int fuid){
    char buf[650] ,code_out[900];
    int fd ,size;
    base64_encodestate state_in;
    base64_init_encodestate(&state_in);
    if((fd = open(filename ,O_RDONLY)) == -1){
        printf("文件不存在或无读取权限");
        return 0;
    }
    while(1){
        size = read(fd ,buf ,sizeof(buf));
        base64_encode_block(buf ,size ,code_out ,&state_in);
        cJSON *root = cJSON_CreateObject();
        cJSON *item = cJSON_CreateString("F");
        cJSON_AddItemToObject(root ,"type" ,item);
        item = cJSON_CreateNumber(gl_uid);
        cJSON_AddItemToObject(root ,"uid",item);
        item = cJSON_CreateNumber(fuid);
        cJSON_AddItemToObject(root ,"fuid" ,item);
        item = cJSON_CreateString(filename);
        cJSON_AddItemToObject(root ,"filename" ,item);
        item = cJSON_CreateNumber(size);
        cJSON_AddItemToObject(root ,"size" ,item);
        item = cJSON_CreateString(code_out);
        cJSON_AddItemToObject(root ,"con",item);
        char *out = cJSON_Print(root);
        cJSON_Delete(root);
        printf("\n%s\n" ,code_out);
        if(send(sock_fd ,out,strlen(out)+1 ,0) <= 0){
            perror("send");
            free(out);
            return 0;
        }
        if(size < sizeof(buf)) break;
        free(out);
    }
    close(fd);
    return 1;
}
void Chat_Srv_InitList(){
    List_Init(PriMsgList ,private_msg_t);
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
int Chat_Srv_SendPrivate(int to_uid ,const char * msg){
    int rtn = 1;
    private_msg_t *NewMsg = (private_msg_t *)malloc(sizeof(private_msg_t));
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
    if(send(sock_fd ,(void *)out ,strlen(out) + 1 ,0) <= 0){
        printf("服务器失去响应\n");
        rtn = 0;
    }
    cJSON_Delete(root);
    free(out);
    return rtn;
}
