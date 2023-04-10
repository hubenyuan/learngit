/*********************************************************************************
 *      Copyright:  (C) 2023 Hu Ben Yuan<2017603273@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  crypto.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(04/08/2023)
 *         Author:  Hu Ben Yuan <2017603273@qq.com>
 *      ChangeLog:  1, Release initial version on "04/08/2023 08:11:44 PM"
 *                 
 ********************************************************************************/

#include <string.h>


int encrypt(char *plaintext, char *ciphertext, int size)
{
	int    i;

	if( size < strlen(plaintext) )
	{
		return -1;
	}

	for(i=0; i<strlen(plaintext); i++)
	{
		ciphertext[i] = plaintext[i]+3;
	}

	return 0;
}

int decrypt(char *ciphertext, char *plaintext, int size)
{
	int   i;

	if( size < strlen(ciphertext) )
	{
		return -1;
	}

	for(i=0; i<strlen(ciphertext); i++)
	{
		plaintext[i] = ciphertext[i]-3;
	}

	return 0;
}

