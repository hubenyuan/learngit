/*********************************************************************************
 *      Copyright:  (C) 2023 Hu Ben Yuan<2017603273@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  my_sqlite3.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(04/18/2023)
 *         Author:  Hu Ben Yuan <2017603273@qq.com>
 *      ChangeLog:  1, Release initial version on "04/18/2023 10:44:26 AM"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#define  list_name "packaged_data"

#include "logger.h"

static sqlite3  *db;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int    i;
    for(i=0; i<argc; i++)
    {
        log_info("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    log_info ("\n");
    return 0;
}


/*创建连接数据库并且创建名为packaged_data的表*/
int get_sqlite_create_db()
{
    char              create_buf[128];
    char             *zErrMsg;
    int               rc;
    char             *sql;

    rc = sqlite3_open("test.db", &db);

    if( rc )
    {
        log_warn("Can't open database: %s\n", strerror(errno));
        return -1;
    }
    else
    {
        log_info("Opened database successfully.\n");
    }

    memset(create_buf,0,sizeof(create_buf));
    sprintf(create_buf,"CREATE TABLE %s(ID INTEGER PRIMARY KEY, TIME CHAR(80),SERIAL CHAR(30),TEMPERATURE CHAR(50));",list_name);
    rc = sqlite3_exec(db,create_buf,callback,0,&zErrMsg);

    if( rc != SQLITE_OK )
    {
        log_warn("failure to create %s: %s\n",list_name,zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    else
    {
        log_info("create %s successfully\n",list_name);
    }

    return 0;
}

/*向数据库表里面插入数据*/
int sqlite_insert_data(char *time_buf, char *serial_buf, char *temp_buf)
{
    char      *zErrMsg;
    int        rc;
    char       insert_buf[256];
    int        rd;
    

    memset(insert_buf, 0, sizeof(insert_buf));
    sprintf(insert_buf,"INSERT INTO %s VALUES( NULL, '%s', '%s', '%s' );", list_name,time_buf, serial_buf, temp_buf);

    rc = sqlite3_exec(db, insert_buf, callback, 0, &zErrMsg);

    if(rc != SQLITE_OK)
    {
        
        log_warn("insert data failure: %s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }

    log_info("Insert  data successfully\n");
    return 0;
}

/*获取数据库数据最大ID并且判断数据库里面存不存在数据*/
int sqlite_maxid(int * maxid)
{
    char     *zErrMsg;
    char    **result;
    int       rownum;
    int       colnum;
    int       rc;
    char     *p;
    char      maxid_buf[128];
    

    memset(maxid_buf,0,sizeof(maxid_buf));
    sprintf(maxid_buf,"SELECT count(*) from %s",list_name);
    rc = sqlite3_get_table(db, maxid_buf, &result, &rownum, &colnum, &zErrMsg);

    if(rc != SQLITE_OK)
    {
        log_warn("query records count from database failure: %s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }

    if( rownum <= 0)
    {
        log_warn("Obtaining maxid failure: %s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        return -2;
    }
    *maxid = atoi(result[1*colnum]);
    return 0;
}


/* 获取数据库表里面的内容并且发送到服务器 */ 
int sqlite_select_data(char *send_buf)
{
    char            select_buf[128];
    char           *zErrMsg;
    int             rc;
    int             counts;
    int             rownum;
    int             colnum;
    char          **result;

    memset(select_buf,0,sizeof(select_buf));

    sprintf(select_buf,"SELECT * FROM %s ORDER BY id DESC;",list_name);

    rc = sqlite3_get_table(db, select_buf, &result, &rownum, &colnum, &zErrMsg);
    if(rc != SQLITE_OK)
    {
        log_warn("Obtaining data failure: %s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    sprintf(send_buf,"%s/%s/%s",result[1*colnum+1],result[1*colnum+2],result[1*colnum+3]);

    return 0;
}

/* 删除数据库表里面ID最大的数据 */
int sqlite_delete_data()
{
    int           rc;
    char         *zErrMsg;
    char          delete_buf[128];
    
    memset(delete_buf,0,sizeof(delete_buf));
    sprintf(delete_buf,"DELETE FROM %s WHERE ID=(SELECT MAX(ID) FROM %s);",list_name,list_name);

    rc = sqlite3_exec(db, delete_buf, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK )
    {
        log_warn("delete packaged_data failure: %s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    log_info ("delete packaged_data successfully\n");
    return 0;
}

int sqlite_close_db(void)
{
    char       *zErrMsg;
    int         rc;

    rc = sqlite3_close(db);
    if(rc != SQLITE_OK)
    {
        log_warn("close database failure: %s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    log_info("close database successfully\n");
    return 0;
}
