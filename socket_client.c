
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>

#include "package_data.h"
#include "my_sqlite3.h"

static inline void print_usage(char *progname)
{
    printf("%s usage: \n", progname);
    printf("-i[ipaddr ]: sepcify server IP address\n");
    printf("-p[port   ]: sepcify server port.\n");
    printf("-t[times  ]: sepcify the time to send.\n");
    printf("-h[help   ]: print this help informstion.\n");
    return ;
}

int main(int argc, char **argv)
{
	int                 sockfd = -1;
	int                 rv = -1;
	struct sockaddr_in  servaddr;
	char               *servip = NULL;
	int                 port = 0;
	int                 times = 3;
	char                buf[512];
	char                data_buff[512];
	int                 ch;
	int                 maxid;
	int                 idx;
	int                 founds = 0;
	char                time_buf[64];
	char                serial_buf[64];
	char                temp_buf[64];
	char                send_buf[128];

	struct option       opts[] = {
		{"ipaddr", required_argument, NULL, 'i'},
		{"port", required_argument, NULL, 'p'},
		{"times", required_argument, NULL, 't'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	while((ch=getopt_long(argc, argv, "i:p:t:h", opts, &idx)) != -1)
	{
		switch(ch)
		{
			case 'i':
				servip=optarg;
				break;
			case 'p':
				port=atoi(optarg);
				break;
			case 't':
				times=atoi(optarg);
				break;
			case 'h':
				print_usage(argv[0]);
				return 0;
		}
	}
	if( argc < 3)
	{
		printf("Program usage: %s [ServerIP] [Port] [Time]\n",argv[0]);
		return 0;
	}

	servip=argv[1];
	port = atoi(argv[2]);

	/*创建连接数据库和并且创建名为packaged_data的表*/
	get_sqlite_create_db();
	while(1)
	{
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0)
		{
			printf("Create socket failur %s\n", strerror(errno));
			close(sockfd);
			return -1;
		}
		printf("Create socket[%d] successfully!\n",sockfd);

		memset(&servaddr, 0, sizeof(servaddr));
		servaddr.sin_family=AF_INET;
		servaddr.sin_port = htons(port);
		inet_aton(servip, &servaddr.sin_addr);

		rv = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
		if(rv < 0)
		{
			printf("Connect to server[%s:%d] failure: %s\n",servip, port, strerror(errno));
            /* 获取当前时间 */
			get_time(time_buf);
			/* 获取温度 */ 
			get_temporary(temp_buf);
			/* 获取产品序列号 */
			get_serial(serial_buf);

		    sqlite_insert_data(time_buf, serial_buf, temp_buf);
			sleep(5);
			continue;
		}
		printf("Connect to server[%s:%d] successfully!\n", servip, port);

		/*  获取数据库数据最大ID并且判断数据库里面存不存在数据 */
		sqlite_maxid( &maxid );
		/*  maxid > 0表示有数据 */
		if(maxid>0)
		{
			memset(send_buf, 0, sizeof(send_buf));
			memset(data_buff, 0, sizeof(data_buff));
			/* 获取数据库表里面的数据并且发送到服务器 */
			sqlite_select_data(send_buf);
			sprintf(data_buff,"%s\n",send_buf);
			rv=write(sockfd, data_buff, strlen(data_buff));
			/* 删除数据库表里面ID最大的数据  */
			sqlite_delete_data();
			continue;
		}

		while(1)
		{
			/* 获取当前时间*/ 
			get_time(time_buf);
			/* 获取温度*/
			get_temporary(temp_buf);
			/* 获取产品序列号 */
			get_serial(serial_buf);

			memset(data_buff,0,sizeof(data_buff));
			sprintf(data_buff,"%s/%s/%s\n",time_buf,serial_buf,temp_buf);

			sleep(times);
			rv=write(sockfd, data_buff, strlen(data_buff));
			if(rv < 0 )
			{
				printf("write to server by socket[%d] failure: %s\n",sockfd,strerror(errno));
				break;
			}

			memset(buf, 0, sizeof(buf));
			rv=read(sockfd, buf, sizeof(buf));	
			if(rv < 0 )
			{
				printf("Read data from server by sockfd[%d] failure: %s\n",sockfd, strerror(errno));
				break;
			}
			else if( rv == 0 )
			{
				printf("socket[%d] get disconnected\n",sockfd);
				break;
			}
			else if( rv > 0 )
			{	
				printf("Read %d bytes data from Server.\n",rv);
			}
		}
	}

	sqlite_close_db();
	close(sockfd);
	return 0;
}
