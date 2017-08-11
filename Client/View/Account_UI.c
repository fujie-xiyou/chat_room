/*************************************************************************
	>    File Name: Account_UI.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月11日 星期五 08时42分57秒
 ************************************************************************/

#include <stdio.h>
#include "./Account_UI.h"
#include "../Service/Account_Srv.h"
int Account_UI_SignIn(){
    char name[30] , password[30];
    printf("请输入要注册的用户名:");
    scanf("%s",name);
    fflush(stdin);
    printf("请输入密码:");
    scanf("%s",password);
    fflush(stdin);
    return Account_Srv_SignIn(name , password);
}
