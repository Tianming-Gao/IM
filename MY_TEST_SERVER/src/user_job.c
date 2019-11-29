#include <thread_pool.h>

void* user_job(void* arg)
{
	printf("user job is running\n");
	int clientfd = *(int*)arg;
	char buf[1500];
	int len = read(clientfd,buf,sizeof(buf));
	int j;
	j=0;
	while(len > j)
	{
		buf[j] = toupper(buf[j]);
		j++;
	}
	write(clientfd,buf,len);

	bzero(buf,sizeof(buf));
	return NULL;
}
