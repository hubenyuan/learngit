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
int callback_function(void *data, int argc, char **argv, char **col_names)
{
	int   *counts = (int *)data;
	*counts = atoi(argv[0]);
	return 0;
}

/*创建连接数据库并且创建名为packaged_data的表*/
int get_sqlite_create_db()
{
	sqlite3          *db;
    char              create_buf[256];
    char             *zErrMsg;
    int               rc;
	char              *sq;

    rc = sqlite3_open("test.db", &db);

    if( rc )
    {
    	printf("Can't open database: %s\n", strerror(errno));
        return -1;
    }
    else
    {
        printf("Opened database successfully.\n");
    }

	sq  = "CREATE TABLE packaged_data("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"TIME CHAR(80),"
		"SERIAL CHAR(30),"
		"TEMPERATURE CHAR(50));";
	rc = sqlite3_exec(db,sq,callback,0,&zErrMsg);

	if( rc != SQLITE_OK )
    {
        printf("failure to create packaged_data: %s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    else
    {
        printf("create packaged_data successfully\n");
    }
	sqlite3_close(db);

    return 0;
}

/*向数据库表里面插入数据*/
int sqlite_insert_data(char *time_buf, char *serial_buf, char *temp_buf)
{
	sqlite3   *db;
	char      *zErrMsg;
	int        rc;
	char       insert_buf[256];
	int        rd;
    
	rd = sqlite3_open("test.db", &db);
	if( rd != SQLITE_OK )
	{
		printf("open test.db failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}

	memset(insert_buf, 0, sizeof(insert_buf));
	sprintf(insert_buf,"INSERT INTO packaged_data VALUES( NULL, '%s', '%s', '%s' );", time_buf, serial_buf, temp_buf);

	rc = sqlite3_exec(db, insert_buf, callback, 0, &zErrMsg);

	if(rc != SQLITE_OK)
	{
		
		printf("insert data failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}

	//printf("%s\n",insert_buf);
	printf ("Insert  data successfully\n");
	sqlite3_close(db);
	return 0;
}

/*获取数据库数据最大ID*/
/*
int sqlite_maxid(int i)
{
	sqlite3  *db;
	char     *zErrMsg;
	char    **result;
	int       rownum;
	int       colnum;
	int       rc,rd;
	int       maxid=0;
	char      maxid_buf[128];
	
	rd = sqlite3_open("test.db",&db);
	if(rd != SQLITE_OK)
	{
		printf("open test.db failure: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	memset(maxid_buf, 0, sizeof(maxid_buf));
	sprintf(maxid_buf,"selete count(*) from packaged_data");
	rc = sqlite3_get_table(db, maxid_buf, &result, &rownum, &colnum, &zErrMsg);

	if(rc != SQLITE_OK)
	{
		printf("Obtaining maxid failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}

	if( rownum <= 0)
	{
		printf("Obtaining maxid failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -2;
	}

	maxid = atoi(result[1*colnum]);
	i = maxid;
	return 0;
}
*/

/* 获取数据库表里面的内容并且发送到服务器 */ 
int sqlite_select_data(char *send_buf, int i)
{
	sqlite3        *db;
	sqlite3_stmt   *stmt;
	char           *zErrMsg;
	char           *select;
	char           *sql;
	int             rc;
	int             rd;
	int             rs;
	int             counts;
	int             rownum;
	int       		colnum;
	char    	  **result;

	rd = sqlite3_open("test.db", &db);
	if( rd != SQLITE_OK )
	{
		printf("open test.db failure: %s\n", zErrMsg);
		return -1;
	}

	sql ="SELECT COUNT(*) FROM packaged_data";


	select = "SELECT * FROM packaged_data ORDER BY id DESC LIMIT 1;";

	rs = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(rs != SQLITE_OK)
	{
		printf("Failure to execute qurey:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -2;
	}

	if(sqlite3_step(stmt) == SQLITE_ROW)
	{
		counts = sqlite3_column_int(stmt,0);
	}
	sqlite3_finalize(stmt);

	if(counts > 0)
	{
	    rc = sqlite3_get_table(db, select, &result, &rownum, &colnum, &zErrMsg);
	    sprintf(send_buf,"%s/%s/%s\n",result[1*colnum+1],result[1*colnum+2],result[1*colnum+3]);
	}
	else
	{
		i = -1;
		printf("Sqlite3 all data has been transferred!\n");
		return -3;
	}
	sqlite3_close(db);
	return 0;
}
/* 	rc = sqlite3_get_table(db, select, &result, &rownum, &colnum, &zErrMsg);

	printf("%d\n",rc);
	if(rc != SQLITE_OK)
	{
		printf("select data failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	sprintf(send_buf,"%s/%s/%s\n",result[1*colnum+1],result[1*colnum+2],result[1*colnum+3]);
	sqlite3_close(db);
	return 0;
*/


/* 删除数据库表里面的内容 */
int sqlite3_delete_data()
{
	sqlite3      *db;
	sqlite3_stmt *stmt;
	sqlite3_stmt *delete_stmt;
	int           rc;
	char         *delete_sql;
	char         *zErrMsg;
	int           rd;
	char         *sql;
	char          delete_buf[128];
	int           max_id;

	rd = sqlite3_open("test.db",&db);
	if( rd != SQLITE_OK )
	{
		printf("open test.db failure: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return 0;
	}

	delete_sql = "DELETE FROM packaged_data WHERE ID=(SELECT MAX(ID) FROM packaged_data);"\
				  "SELECT *FROM packaged_data;";

/*
	sql = "SELECT id FROM packaged_data WHERE ORDER BY id DESC LIMIT 1;";

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) 
	{
		max_id = sqlite3_column_int(stmt, 0);
	}

	delete_sql = "DELETE FROM table_name WHERE id = ?;"\
				  "SELECT * FROM packaged_data WHERE ORDER BY id DESC LIMIT 1;";
	rc =sqlite3_prepare_v2(db, delete_sql, -1, &delete_stmt, NULL);
	if(rc == SQLITE_OK)
	{
		sqlite3_bind_int(delete_stmt, 1, max_id);
		sqlite3_step(delete_stmt);
	}
	sqlite3_finalize(delete_stmt);
*/
    rc = sqlite3_exec(db, delete_sql, callback, 0, &zErrMsg);

	if( rc != SQLITE_OK )
	{
		printf ("delete packaged_data failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	printf ("delete packaged_data successfully\n");
	sqlite3_close(db);
	return 0;
}
