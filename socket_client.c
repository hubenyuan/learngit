

/*
#include <sqlite3.h>
#define  table_name "cli_temp"
*/
#include "socket_client.h"
#include "package_data.h"


int main(int argc, char **argv)
{
	int                 sockfd = -1;
	int                 rv = -1;
	struct sockaddr_in  servaddr;
	char               *servip = NULL;
	int                 port = 0;
	int                 times = 3;
	char                buf[1024];
	char                data_buff[1024];
	int                 ch;
	int                 idx;
	int                 founds = 0;
	char                time_buf[64];
	char                serial_buf[64];
	char                temp_buf[64];

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
			sleep(5);
			continue;
		}
		printf("Connect to server[%s:%d] successfully!\n", servip, port);

		while(1)
		{
			/*获取当前时间*/
			get_time(time_buf);
			/*获取温度*/
			get_temporary(temp_buf);
			/*获取产品序列号*/
			get_serial(serial_buf);
			/*把三个数据打包到一起*/
			sprintf(data_buff,"%s/%s/%s", time_buf,serial_buf,temp_buf);

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
				printf("Read %d bytes data from Server: %s\n",rv, buf);
			}
		}
	}
	close(sockfd);
	return 0;
}
