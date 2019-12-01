#include "myepoll.h"
CMyEpoll::CMyEpoll()
{
}
CMyEpoll::~CMyEpoll()
{
}

void CMyEpoll::epoll_run()
{
	m_tp = new CThread_Pool();
	serverfd = m_tp->thread_socket_init();
	m_tp->m_threadpool = m_tp->thread_pool_create(10,50,100);
	epfd = epoll_create(EPOLL_MAX);
	tmpep.events = EPOLLIN;
	tmpep.data.fd = serverfd;
	tmpep.events |= EPOLLET;
	epoll_ctl(epfd,EPOLL_CTL_ADD,serverfd,&tmpep);
	int i;
	while(m_tp->m_threadpool->thread_shutdown)
	{
		ready = epoll_wait(epfd,readyarr,EPOLL_MAX,-1);
		while(ready)
		{
			i=0;
			if(readyarr[i].data.fd == serverfd)
			{
				size = sizeof(clientaddr);
				clientfd = accept(readyarr[i].data.fd,(struct sockaddr*)&clientaddr,&size);
				cout << "accepting .." << endl;
				tmpep.data.fd = clientfd;
				tmpep.events = EPOLLIN;
				tmpep.events |= EPOLLET;
				epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&tmpep);
			}
			else
			{
				tmptask.job = m_tp->user_job;
				tmptask.arg = (void*)&readyarr[i].data.fd;
				thread_add_task(tmptask);	
				cout << "add to task queue.." << endl;
			}
			ready--;
			i++;
		}
	}
	pthread_exit(NULL);
}
