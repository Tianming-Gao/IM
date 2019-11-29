#include <thread_pool.h>

int Daemon_job()
{
	pool_t *p;
	task_t tmptask;
	int ready;
	int serverfd = thread_socket_init();
	p =thread_pool_create(10,50,100);
	int epfd = epoll_create(EPOLL_MAX);
	struct epoll_event readyarr[EPOLL_MAX];
	struct epoll_event tmpep;
	tmpep.events = EPOLLIN;
	tmpep.events |= EPOLLET;
	tmpep.data.fd = serverfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,serverfd,&tmpep);
	int i;
	int clientfd;
	struct sockaddr_in clientaddr;
	socklen_t size;
	while(p->thread_shutdown)
	{
		ready = epoll_wait(epfd,readyarr,EPOLL_MAX,-1);
		while(ready)
		{
			i=0;
			if(readyarr[i].data.fd == serverfd)
			{
				size = sizeof(clientaddr);
				clientfd = accept(readyarr[i].data.fd,(struct sockaddr*)&clientaddr,&size);
			
				tmpep.data.fd = clientfd;
				tmpep.events = EPOLLIN;
				tmpep.events |= EPOLLET;
				epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&tmpep);
			}
			else
			{
				tmptask.job = user_job;
				tmptask.arg = (void*)&readyarr[i].data.fd;
				thread_add_task(p,tmptask);
			}
			ready--;
			i++;
		}
	}
	pthread_exit(NULL);
}
