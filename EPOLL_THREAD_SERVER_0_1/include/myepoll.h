#ifndef EPOLL_H
#define EPOLL_H
#include "sys.h"
#include "thread_pool.h"
class CMyEpoll:public CThread_Pool
{
	public:
		CMyEpoll();
		~CMyEpoll();
	private:
		CThread_Pool* m_tp;
		task_t tmptask;
		int ready;
		int serverfd;
		int epfd;
		struct epoll_event readyarr[EPOLL_MAX];
		struct epoll_event tmpep;
		int clientfd;
		struct sockaddr_in clientaddr;
		socklen_t size;
	public:
		void epoll_run();
};

#endif
