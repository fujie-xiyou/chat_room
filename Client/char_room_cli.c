/*************************************************************************
	>    File Name: char_room_cli.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月11日 星期五 08时58分20秒
 ************************************************************************/

#include<stdio.h>
#include "./View/Main_UI.h"
#include "./Service/Connect.h"
int gl_uid;//记录登录用户的uid
int main(){
    Connect();
    Main_UI_Hello();
}
