/*********************************************************************************
 *      Copyright:  (C) 2023 Hu Ben Yuan<2017603273@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  package_data.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(04/18/2023)
 *         Author:  Hu Ben Yuan <2017603273@qq.com>
 *      ChangeLog:  1, Release initial version on "04/18/2023 09:02:35 PM"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "logger.h"

struct t 
{
	int tm_sec;              // 秒 – 取值区间为[0,59]  　　
    int tm_min;              // 分 - 取值区间为[0,59]  　　
    int tm_hour;             // 时 - 取值区间为[0,23]  　　
    int tm_mday;             // 一个月中的某日 - 取值区间为[1,31] 
    int tm_mon;              // 月份 - 取值区间为[0,11]，（0为一月）
    int tm_year;             // 年份 　
};

int get_temperature(float *temp)
{
    int              fd=-1;
    char             buf[128];
    char            *ptr=NULL;
    DIR             *dirp=NULL;
    struct dirent   *direntp=NULL;
    char             w1_path[64]="../../w1/device/";
    char             chip_sn[32];
    int              found=0;
    char             ds18b20_path[64];
    dirp=opendir(w1_path);
        
    if(!dirp)
    {
        log_error("open folder %s failure: %s\n",w1_path,strerror(errno));
        return -1;
    }
        
    while(NULL!=(direntp=readdir(dirp)))
    {
        if(strstr(direntp->d_name,"28-"))
        {
            strncpy(chip_sn,direntp->d_name,sizeof(chip_sn));
            found=1;
        }
    }

    closedir(dirp);

    if(!found)
    {
        log_error("can not find ds18b20_path chipest\n");
        return -2;
    }
    strncat(w1_path,chip_sn,sizeof(w1_path)-strlen(w1_path));
    strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen(w1_path));
    fd=open(w1_path,O_RDONLY);
        
    if(fd<0)
    {
        log_warn("open file failure:%s\n",strerror(errno));
        perror("open file failure\n");
        return -3;
    }

    memset(buf,0,sizeof(buf));

    if(read(fd,buf,sizeof(buf))<0)
    {
        log_warn("read data from fd=%d failure: %s\n",fd,strerror(errno));
        return -4;
    }
    ptr=strstr(buf,"t=");
        
        if(!ptr)
    {
        log_warn("can not find t=string\n");
        return -5;
    }
        
        ptr+=2;
    *temp=atof(ptr)/1000;
    close(fd);
    return 0;
}


/*获取时间*/
int get_time(char *time_buf)
{
	time_t   now;
	struct   tm *t;
	time(&now);
	t = localtime(&now);
	if(!t)
	{
		log_warn("failure: %s\n",strerror(errno));
		return -1;
	}

	memset(time_buf, 0, sizeof(time_buf));
	sprintf(time_buf,"%04d-%02d-%02d %02d:%02d:%02d",
			t->tm_year+1900,t->tm_mon+1,t->tm_mday,
			t->tm_hour,t->tm_min,t->tm_sec);
	return 0;
} 

/*获取产品序列号*/
int get_serial(char *serial_buf)
{
	struct dirent  *entry;
	int             founds = 0;
    DIR            *dirptr;
    dirptr = opendir("../../w1/device/");

	memset(serial_buf, 0, sizeof(serial_buf));
	if( (entry = readdir(dirptr)) == NULL )
    {
        log_warn("open file failure: %s\n",strerror(errno));
                return -1;
    }

    while( (entry = readdir(dirptr)) != NULL )
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strstr(entry->d_name,"28-"))
        {
			sprintf(serial_buf,"%s",entry->d_name);
            founds = 1;
        }
				
    }
    if( !founds )
    {
        log_error("can not find ds18b20_path chipest\n");
        return -2;
    }
	closedir(dirptr);
    return 0;
}

/*获取温度*/
int get_temporary(char *temp_buf)
{
	float   temp;
    int     rs = -1;
    rs=get_temperature(&temp);
    memset(temp_buf, 0, sizeof(temp_buf));
	if(rs<0)
    {
		log_warn("get temperature failure,return value:%d",rs);
        return -1;
    }
	sprintf(temp_buf,"%f",temp);
	return 0;

}
