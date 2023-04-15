/*********************************************************************************
 *      Copyright:  (C) 2023 Hu Ben Yuan<2017603273@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  main.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(04/15/2023)
 *         Author:  Hu Ben Yuan <2017603273@qq.com>
 *      ChangeLog:  1, Release initial version on "04/15/2023 10:52:00 AM"
 *                 
 ********************************************************************************/

#include "ds18b20.h"

int main(int argc, char **argv)
{
	time_t          now;
    float           temp;
    int             rv;
    struct tm      *t;
    DIR            *dirptr;
    struct dirent  *entry;

    time(&now);
    t = localtime(&now);
    rv=get_temperature(&temp);
    dirptr = opendir("../w1/device/");
        
    while( (entry = readdir(dirptr)) != NULL )
    {
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
        {
			printf("%s/", entry->d_name);
        }
    }
    closedir(dirptr);

    if(rv<0)
    {
        printf("get temperature failure,return value:%d\n",rv);
        return -1;
    }
    printf("%d;%d;%d,",t->tm_year+1900,t->tm_mon+1,t->tm_mday);
    printf("%d:%d:%d/",t->tm_hour,t->tm_min,t->tm_sec);
    printf("%f\n",temp);

    return 0;

}

