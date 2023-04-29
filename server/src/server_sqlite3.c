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
#define  list_name "server_data"

sqlite3     *db;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int    i;
    for(i=0; i<argc; i++)
    {
        printf ("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf ("\n");
    return 0;
}


/*创建连接数据库并且创建名为packaged_data的表*/
int get_sqlite_create_db()
{
    char              create_buf[128];
    char             *zErrMsg;
    int               rc;
	char              *sql;

    rc = sqlite3_open("server.db", &db);

    if( rc )
    {
    	printf("Can't open database: %s\n", strerror(errno));
        return -1;
    }
    else
    {
        printf("Opened database successfully.\n");
    }

	memset(create_buf,0,sizeof(create_buf));
	sprintf(create_buf,"CREATE TABLE %s(ID INTEGER PRIMARY KEY, time CHAR(80),serial CHAR(30),temperature CHAR(50));",list_name);
	rc = sqlite3_exec(db,create_buf,callback,0,&zErrMsg);

	if( rc != SQLITE_OK )
    {
        printf("failure to create %s: %s\n",list_name,zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    else
    {
        printf("create %s successfully\n",list_name);
    }

    return 0;
}

/*向数据库表里面插入数据*/
int sqlite_insert_data(char *buf)
{
	char      *zErrMsg;
	int        rc;
	char       insert_buf[512];
	int        rd;
	char      *str1,*str2,*str3;
	str1 = strtok(buf,"/");
	str2 = strtok(NULL,"/");
	str3 = strtok(NULL,"/");

	memset(insert_buf, 0, sizeof(insert_buf));
	sprintf(insert_buf,"INSERT INTO %s VALUES( NULL, '%s', '%s', '%s' );", list_name,str1, str2, str3);

	rc = sqlite3_exec(db, insert_buf, callback, 0, &zErrMsg);

	if(rc != SQLITE_OK)
	{
		
		printf("insert data failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}

	printf ("Insert  data successfully\n");
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
		printf("query records count from database failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}

	if( rownum <= 0)
	{
		printf("Obtaining maxid failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -2;
	}
	*maxid = atoi(result[1*colnum]);
	return 0;
}


/* 获取数据库表里面的数据并且打印出来 */ 
int sqlite_select_data(void)
{
	char            select_buf[128];
	char           *zErrMsg;
	int             rc;
	int             counts;
	int             rownum;
	int       		colnum;
	char    	  **result;

	memset(select_buf,0,sizeof(select_buf));

	sprintf(select_buf,"SELECT * FROM %s;",list_name);

	rc = sqlite3_exec(db, select_buf, callback, NULL, &zErrMsg);
	if(rc != SQLITE_OK)
	{
		printf("select  data failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}

	return 0;
}

/* 删除数据库表里面的数据 */
int sqlite_delete_data(void)
{
	int           rc;
	char         *zErrMsg;
	char          delete_buf[128];
	
	memset(delete_buf,0,sizeof(delete_buf));
	sprintf(delete_buf,"DELETE FROM %s;",list_name);

    rc = sqlite3_exec(db, delete_buf, callback, 0, &zErrMsg);

	if( rc != SQLITE_OK )
	{
		printf ("delete server_data failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	printf ("delete packaged_data successfully\n");
	return 0;
}

int sqlite_close_db(void)
{
	char       *zErrMsg;
	int         rc;

	rc = sqlite3_close(db);
	if(rc != SQLITE_OK)
	{
		printf("close database failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	printf("close database successfully\n");
	return 0;
}
