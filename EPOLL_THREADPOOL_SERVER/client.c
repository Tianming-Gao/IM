#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
	struct sockaddr_in serveraddr;
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8000);
	inet_pton(AF_INET,"192.168.122.133",&serveraddr.sin_addr.s_addr);

	int sockfd = socket(AF_INET,SOCK_STREAM,0);

	socklen_t size = sizeof(serveraddr);
	if((connect(sockfd,(struct sockaddr*)&serveraddr,size)) == 0)
	{
		printf("connect success..\n");
	}
	int len;
	char buf[1500];
	while((fgets(buf,sizeof(buf),stdin))!= NULL)
	{
		write(sockfd,buf,strlen(buf));
		bzero(buf,sizeof(buf));
		len = read(sockfd,buf,sizeof(buf));
		printf("result:%s\n",buf);
		bzero(buf,sizeof(buf));
	}
	return 0;
}
