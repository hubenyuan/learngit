/*********************************************************************************
 *      Copyright:  (C) 2023 Hu Ben Yuan<2017603273@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  test.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(04/16/2023)
 *         Author:  Hu Ben Yuan <2017603273@qq.com>
 *      ChangeLog:  1, Release initial version on "04/16/2023 07:46:00 PM"
 *                 
 t *******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <errno.h>
#include <time.h>

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
	char              buf[1024];
	char *zErrMsg 	  = 0;
	int               rc;

	rc = sqlite3_open("test.db", &db);

	if( rc )
	{
		printf("Can't open database: %s\n", strerror(errno));
		exit(0);
	}
	else
	{
		printf("Opened database successfully.\n");
	}
	//sqlite3_close(db);

	rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS packaged_data ("
				"ID  INTEGER PRIMARY KEY AUTOINCREMENT,"
				"SERIAL NO TEXT"
				"TIME TEXT"
				"TEMPERATURE TEXT"
				");",NULL,NULL,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		printf("failure to create packaged_data: %s",zErrMsg);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return 1;
	}
	else
	{
		printf("create packaged_data successfully\n");
	}

	sqlite3_close(db);
	return 0;
}
