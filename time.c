/*********************************************************************************
 *      Copyright:  (C) 2023 Hu Ben Yuan<2017603273@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  time.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(04/11/2023)
 *         Author:  Hu Ben Yuan <2017603273@qq.com>
 *      ChangeLog:  1, Release initial version on "04/11/2023 10:53:16 AM"
 *                 
 
********************************************************************************/
#include <stdio.h>
#include <time.h>

struct t 
{
	int tm_sec;		 	 // 秒 – 取值区间为[0,59]  　　
	int tm_min; 		 // 分 - 取值区间为[0,59]  　　
	int tm_hour; 	     // 时 - 取值区间为[0,23]  　　
	int tm_mday;		 // 一个月中的某日 - 取值区间为[1,31] 
	int tm_mon;		     // 月份 - 取值区间为[0,11]，（0为一月）
	int tm_year; 	     // 年份 　
};


int main(int argc, char **argv)
{
	time_t now;
	struct tm *t;

	time(&now);
	t = localtime(&now);
	printf("%d;%d;%d ",t->tm_year+1900,t->tm_mon+1,t->tm_mday);
	printf("%d:%d:%d\n",t->tm_hour,t->tm_min,t->tm_sec);
	return 0;
}

