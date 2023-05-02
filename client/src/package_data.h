/********************************************************************************
 *      Copyright:  (C) 2023 Hu Ben Yuan<2017603273@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  package_data.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(04/18/2023)
 *         Author:  Hu Ben Yuan <2017603273@qq.com>
 *      ChangeLog:  1, Release initial version on "04/18/2023 10:22:57 PM"
 *                 
 ********************************************************************************/
#ifndef PACKAGE_DATA_H
#define PACKAGE_DATA_H

/* 获取当前的时间 */
int get_time(char *time_buf);

/* 获取温度 */
int get_temporary(char *temp_buf);

#endif

