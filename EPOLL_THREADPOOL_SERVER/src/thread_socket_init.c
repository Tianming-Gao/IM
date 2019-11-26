#include <thread_pool.h>
int thread_socket_init()
{
	struct sockaddr_in serveraddr;
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int serverfd = socket(AF_INET,SOCK_STREAM,0);
	bind(serverfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	listen(serverfd,128);
	return serverfd;
}
