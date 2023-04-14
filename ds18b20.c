#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include <time.h>
#include <errno.h> 


int get_temperature(float *temp);

struct t
{
	int tm_sec;               // 秒 – 取值区间为[0,59]  　　
	int tm_min;              // 分 - 取值区间为[0,59]  　　
    int tm_hour;            // 时 - 取值区间为[0,23]  　
	int tm_mday;           // 一个月中的某日 - 取值区间为[1,31] 
	int tm_mon;           // 月份 - 取值区间为[0,11]，（0为一月）
	int tm_year;         // 年份 　
};

int main(int argc, char **argv)
{

	time_t          now;
	float           temp;
    int             rv;
	struct tm      *t;
	DIR            *dirptr;
	struct dirent  *entry;

	time(&now);
	t = localtime(&now);
    rv=get_temperature(&temp);
	dirptr = opendir("../w1/device/");
        
	while( (entry = readdir(dirptr)) != NULL )
	{
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
		{
			printf("%s/", entry->d_name);
		}
	}
	closedir(dirptr);

	if(rv<0)
	{
		printf("get temperature failure,return value:%d\n",rv);
        return -1;
    }
	printf("%d;%d;%d,",t->tm_year+1900,t->tm_mon+1,t->tm_mday);
	printf("%d:%d:%d/",t->tm_hour,t->tm_min,t->tm_sec);
	printf("%f\n",temp);

	return 0;

}


int get_temperature(float *temp)
{
	int              fd=-1;
    char             buf[128];
    char            *ptr=NULL;
    DIR             *dirp=NULL;
    struct dirent   *direntp=NULL;
	char             w1_path[64]="../w1/device/";
    char             chip_sn[32];
    int              found=0;
    char             ds18b20_path[64];
      /*char    w1_path[64]="/sys/bus/w1/devices/";*/    
	dirp=opendir(w1_path);
        
	if(!dirp)
    {
		printf("open folder %s failure: %s\n",w1_path,strerror(errno));
        return -1;
    }
        
	while(NULL!=(direntp=readdir(dirp)))
	{
		if(strstr(direntp->d_name,"28-"))
		{
			strncpy(chip_sn,direntp->d_name,sizeof(chip_sn));
            found=1;
		}
	}

	closedir(dirp);

	if(!found)
    {
		printf("can not find ds18b20_path chipest\n");
        return -2;
    }
	strncat(w1_path,chip_sn,sizeof(w1_path)-strlen(w1_path));
	strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen(w1_path));
	//printf("w1_path: %s\n",w1_path);
    fd=open(w1_path,O_RDONLY);
        
	if(fd<0)
	{
		printf("open file failure:%s\n",strerror(errno));
        perror("open file failure\n");
        return -3;
    }

    memset(buf,0,sizeof(buf));

    if(read(fd,buf,sizeof(buf))<0)
    {
        printf("read data from fd=%d failure: %s\n",fd,strerror(errno));
        return -4;
    }
    //printf("buf:%s\n",buf);
    ptr=strstr(buf,"t=");
        
	if(!ptr)
    {
        printf("can not find t=string\n");
        return -5;
    }
        
	ptr+=2;
    *temp=atof(ptr)/1000;
    //printf("temprature:%f\n",*temp);
    close(fd);
    return 0;
}
