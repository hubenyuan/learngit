
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

#define MSG_STR "Hello Ling iot studio\n"

static inline void print_usage(char *progname);

static inline void print_usage(char *progname)
{
	printf("%s usage: \n", progname);
	printf("-i[ipaddr ]: sepcify server IP address\n");
	printf("-p[port   ]: sepcify server port.\n");
	printf("-t[time   ]: sepcify the time to send.\n");
	printf("-h[help    ]: print this help informstion.\n");
	return ;
}


int main(int argc, char **argv)
{
	int                 sockfd = -1;
	int                 rv = -1;
	struct sockaddr_in  servaddr;
	char               *servip = NULL;
	int                 port = 0;
	int                 time = 1;
	char                buf[1024];
	struct option       opts[] = {
		{"ipaddr", required_argument, NULL, 'i'},
		{"port", required_argument, NULL, 'p'},
		{"time", required_argument, NULL, 't'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};
	int       ch;
	int       idx;

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
				time=atoi(optarg);
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
		printf("time:%d\n",time);

		servip=argv[1];
		port = atoi(argv[2]);
		//time = atoi(argv[3]);
		printf("time1:%d\n",time);

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
			close(sockfd);
			return -2;
		}
		printf("Connect to server[%s:%d] successfully!\n", servip, port);

		while(1)
		{
			rv=write(sockfd, MSG_STR, strlen(MSG_STR));
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
				printf("time2:%d\n",time);
				sleep(time);
			}
		}

		close(sockfd);
	    return 0;
}
