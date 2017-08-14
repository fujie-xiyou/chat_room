/*************************************************************************
	>    File Name: Account_Srv.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月10日 星期四 17时27分22秒
 ************************************************************************/

#ifndef _ACCOUNT_SRV_H
#define _ACCOUNT_SRV_H
#include "cJSON.h"
#include "Connect.h"
int Account_Srv_SignIn(const char *name , const char *password);

int Account_Srv_Login(const char *name , const char *password);

#endif
