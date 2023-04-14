/*********************************************************************************
 *      Copyright:  (C) 2023 Hu Ben Yuan<2017603273@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  path.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(04/13/2023)
 *         Author:  Hu Ben Yuan <2017603273@qq.com>
 *      ChangeLog:  1, Release initial version on "04/13/2023 05:41:34 PM"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char **argv)
{
	DIR            *dirptr;
	struct dirent  *entry;
	dirptr = opendir("../w1/device/");
	while( (entry = readdir(dirptr)) != NULL )
	{
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
		{
			printf("%s\n", entry->d_name);
		}
	}
	closedir(dirptr);
	return 0;
}
