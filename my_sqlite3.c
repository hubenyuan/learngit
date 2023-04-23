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


int get_sqlite_create_db()
{
	sqlite3          *db;
    char              create_buf[256];
    char             *zErrMsg;
    int               rc;
	char              *sql;

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

	sql = "CREATE TABLE packaged_data("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"TIME CHAR(80),"
		"SERIAL CHAR(30),"
		"TEMPERATURE CHAR(50));";
	rc = sqlite3_exec(db,sql,callback,0,&zErrMsg);

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

int sqlite_insert_data(char *time_buf, char *serial_buf, char *temp_buf)
{
	sqlite3   *db;
	char      *zErrMsg;
	int        rc;
	char      *sql;
	char       insert_buf[256];
	int        rd;
    
	sqlite3_open("test.db", &db);
	if( rd != SQLITE_OK )
	{
		printf("open test.db failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}

	memset(insert_buf, 0, sizeof(insert_buf));
	sprintf(insert_buf,"INSERT INTO packaged_data VALUES( NULL, '%s', '%s', '%s' );", time_buf, serial_buf, temp_buf);

    //sql = sqlite3_mprintf("INSERT INTO packaged_data VALUES( NULL, '%s', '%s', '%s' );", time_buf, serial_buf, temp_buf);

	rc = sqlite3_exec(db, insert_buf, NULL, 0, &zErrMsg);

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

 
int sqlite_select_data()
{
	sqlite3  *db;
	char     *zErrMsg;
	char     *select;
	int       rc = -1;
	int       rd;

	rd = sqlite3_open("test.db", &db);
	if( rd != SQLITE_OK )
	{
		printf("open test.db failure: %s\n", zErrMsg);
		return -1;
	}

	select = "SELECT *FROM packaged_data LIMIT 3";

	rc = sqlite3_exec(db, select, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK)
	{
		printf("get select data failure: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	
	sqlite3_close(db);
	return 0;
}



int sqlite3_delete_data()
{
	sqlite3      *db;
	int           rc = -1;
	char         *zErrMsg;
	char         *delete;
	int           rd;
	
	rd = sqlite3_open("test,db",&db);
	if( rd != SQLITE_OK )
	{
		printf("open test.db failure: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return 0;
	}

	delete = "DELETE FROM packaged_data";

	rc = sqlite3_exec(db, delete, callback, 0, &zErrMsg);

	if(rc<0)
	{
		printf ("delect packaged_data failure: %s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}

	printf ("delect packaged_data successfully\n");
	sqlite3_close(db);
	return 0;
}
