/*********************************************************************************
 *      Copyright:  (C) 2023 Hu Ben Yuan<2017603273@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  path.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(04/11/2023)
 *         Author:  Hu Ben Yuan <2017603273@qq.com>
 *      ChangeLog:  1, Release initial version on "04/11/2023 11:51:05 AM"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>

static char s_filepath[] = "../w1/device/28-0317320a8aff/";

int main(int argc, char **argv)
{
    char *path;

    printf("产品序列号: %s\n",basename(s_filepath));

    free(path);
    
    return 0;
}


