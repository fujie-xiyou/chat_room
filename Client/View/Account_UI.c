/*************************************************************************
	>    File Name: Account_UI.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月11日 星期五 08时42分57秒
 ************************************************************************/

#include <stdio.h>
#include "./Account_UI.h"
#include "../Common/Common.h"
#include "../Service/Account_Srv.h"
int Account_UI_SignIn(){
    char name[30] , password[30];
    int sex;
    printf("请输入要注册的用户名:");
    scanf("%30s",name);
    ffflush();
    while(1){
        printf("请输入性别(男/女):");
        scanf("%30s",password);//懒得再开数组,暂借一下
        ffflush();
        if(strcmp(password ,"男") == 0){
            sex = 1;
            break;
        }
        else if(strcmp(password ,"女") == 0){
            sex = 0;
            break;
        }
        else{
            printf("目前不存在的性别...\n");
        }
    }
    printf("请输入密码:");
    scanf("%s",password);
    ffflush();
    return Account_Srv_SignIn(name ,sex ,password);
}

int Account_UI_Login(){
    char name[30] , password[30];
    printf("请输入用户名:");
    scanf("%s",name);
    ffflush();
    printf("请输入密码:");
    scanf("%s",password);
    ffflush();
    return Account_Srv_Login(name , password);
    
}
