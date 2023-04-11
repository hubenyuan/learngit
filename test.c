/*********************************************************************************
 *      Copyright:  (C) 2023 Kun_<1433729173@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  test.c
 *    Description:  This file for test vim
 *                 
 *        Version:  1.0.0(2023年04月02日)
 *         Author:  Kun_ <1433729173@qq.com>
 *      ChangeLog:  1, Release initial version on "2023年04月02日 15时49分08秒"
 *                 
 ********************************************************************************/
#include <assert.h>
#include <stdio.h>
#include <string.h>

char *strcpy_(char *strd, const char *strs)
{
	  int i;
	  printf ("1\n");
      assert( (strd != NULL) && (strs != NULL) );
      char *str = strd;
      *strd++ = *strs++;
     /* for(i=0;strs[i]!='\0';i++)
	  {
		  strd[i] = strs[i];
	  }
	  */
      printf ("2\n");
      return str;
}

int main()
{
    
   /* 我的第一个 C 程序 */
   char *strd = " ";
   char *strs = "hello";

   int len = strlen( strcpy_(strd,strs) );
   printf("str:%s\n",strd);
   printf("len:%d\n",len);
   
   return 0;
}

