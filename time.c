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
	int tm_wday; 	     // 星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 　
	int tm_yday; 	     // 一年中1月1日开始的天数 – 取值区间为[0,365] 
	int tm_isdst; 	     
	long int tm_gmtoff;	 
	const char *tm_zone; 
};


int main(int argc, char **argv)
{
	time_t now;
	struct tm *t;
	char *wd[7] = {"星期日","星期一","星期二","星期三","星期四","星期五","星期六",};

	time(&now);
	t = localtime(&now);
	printf("%d年%d月%d日 %s ",t->tm_year+1900,t->tm_mon+1,t->tm_mday,wd[t->tm_wday]);
	printf("%d时%d分%d秒\n",t->tm_hour,t->tm_min,t->tm_sec);
	return 0;
}

