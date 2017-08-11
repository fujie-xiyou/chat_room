/*************************************************************************
	>    File Name: Common.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月11日 星期五 15时35分23秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char *sgets(char *str,int len){//安全gets
	fgets(str,len,stdin);
	if(str[strlen(str)-1]=='\n'){
		str[strlen(str)-1]='\0';
	}
	return str;
}
void ffflush(){//清空输入缓冲区函数
	char ch;
	while((ch=getchar())!='\n' && ch!=EOF){
		//printf("(%x)",ch);
	}
}
