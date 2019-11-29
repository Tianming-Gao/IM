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
	//addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_pton(AF_INET,"192.168.157.128",&serveraddr.sin_addr.s_addr);

	inet_pton(AF_INET,"101.200.180.58",&serveraddr.sin_addr.s_addr);
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	//bind(sockfd,(struct sockaddr *)&addr,sizeof(addr));
	socklen_t size = sizeof(serveraddr);
	if((connect(sockfd,(struct sockaddr *)&serveraddr,size))==0)
	{
		printf("|====== www.hamh.club =====|\n");
		printf("|======小写转大写程序======|\n");
		printf("|==========================|\n");
		
		printf("\n请输入要转换的字符串：\n");
	}
	int len;
	char buf[1500];
	while((fgets(buf,sizeof(buf),stdin))!=NULL)
	{
		write(sockfd,buf,strlen(buf));
		bzero(buf,sizeof(buf));
		len = read(sockfd,buf,sizeof(buf));
		printf("转换结果为：%s\n",buf);
		bzero(buf,sizeof(buf));
		printf("请输入要转换的字符串：\n");
	}
	return 0;
}
