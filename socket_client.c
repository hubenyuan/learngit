

#include "socket_client.h"
#include "ds18b20.h"

int main(int argc, char **argv)
{
	int                 sockfd = -1;
	int                 rv = -1;
	struct sockaddr_in  servaddr;
	char               *servip = NULL;
	int                 port = 0;
	int                 times = 3;
	char                buf[1024];
	char                buffer[512];
	int                 ch;
	int                 idx;
	int                 founds = 0;
	time_t              now;
	float               temp;
	int                 tv;
	struct tm          *t;
	DIR                *dirptr;
	struct dirent      *entry;
	time(&now);
	t = localtime(&now);
	tv = get_temperature(&temp);
	dirptr = opendir("../w1/device/");

	struct option       opts[] = {
		{"ipaddr", required_argument, NULL, 'i'},
		{"port", required_argument, NULL, 'p'},
		{"times", required_argument, NULL, 't'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	if( (entry = readdir(dirptr)) == NULL )
	{
		printf("open file failure: %s\n",strerror(errno));
		return -1;
	}
	while( (entry = readdir(dirptr)) != NULL )
	{
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strstr(entry->d_name,"28-") )
		{
			memset(buffer, 0 , sizeof(buffer));
			sprintf(buffer, "%s/%d-%d-%d %d:%d:%d/%f/\n", entry->d_name, t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec, temp);
			founds = 1;
		}
	}

	if( !founds )
	{
		printf("can not find ds18b20_path chipest\n");
		return -2;
	}

	if(tv<0)
	{
		printf("get temperature failure,return value:%d\n",tv);
		return -1;
	}
	closedir(dirptr);

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
			sleep(times);
			rv=write(sockfd, buffer, strlen(buffer));
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
