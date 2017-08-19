/*************************************************************************
	>    File Name: Common.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月11日 星期五 15时35分23秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"./Common.h"
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


/*以下代码copy from TTMS*/
/*以下代码copy from TTMS*/
/*以下代码copy from TTMS*/

//比较日期dt1, dt2的大小。相等返回0，dt1<dt2返回-1，否则1
int DateCmp(user_date_t dt1, user_date_t dt2) {
	if (dt1.year < dt2.year)
		return -1;
	else if (dt1.year == dt2.year && dt1.month < dt2.month)
		return -1;
	else if (dt1.year == dt2.year && dt1.month == dt2.month && dt1.day < dt2.day)
		return -1;
	else if (dt1.year == dt2.year && dt1.month == dt2.month
			&& dt1.day == dt2.day)
		return 0;
	else
		return 1;
}

//获取系统当前日期
user_date_t DateNow() {
	user_date_t curDate;
	time_t now;         //实例化time_t结构
	struct tm *timeNow;         //实例化tm结构指针
	time(&now);
	timeNow = localtime(&now);
	curDate.year=timeNow->tm_year+1900;
	curDate.month=timeNow->tm_mon+1;
	curDate.day=timeNow->tm_mday;

	return curDate;
}

//获取系统当前时间
user_time_t TimeNow(){
	user_time_t curTime;
	time_t now;         //实例化time_t结构
	struct tm *timeNow;         //实例化tm结构指针
	time(&now);
	timeNow = localtime(&now);
	curTime.hour=timeNow->tm_hour;
	curTime.minute=timeNow->tm_min;
	curTime.second=timeNow->tm_sec;
	return curTime;
}
