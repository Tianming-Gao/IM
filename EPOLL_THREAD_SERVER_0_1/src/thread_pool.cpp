#include "thread_pool.h"

CThread_Pool::CThread_Pool()
{
	//m_threadpool = (pool_t*)malloc(sizeof(pool_t));
}

CThread_Pool::~CThread_Pool()
{

}

pool_t* CThread_Pool::thread_pool_create(int min,int max,int quemax)
{
	m_threadpool = (pool_t*)malloc(sizeof(pool_t));
	m_threadpool->thread_max = max;
	m_threadpool->thread_min = min;
	m_threadpool->queue_max = quemax;

	m_threadpool->thread_alive = 0;
	m_threadpool->thread_busy = 0;
	m_threadpool->thread_shutdown = 1;

	m_threadpool->tids = (pthread_t*)malloc(max * sizeof(pthread_t));
	bzero(m_threadpool->tids,max*sizeof(pthread_t));
	m_threadpool->queue_task = (task_t *)malloc(quemax * sizeof(task_t));
	m_threadpool->queue_size = 0;
	m_threadpool->queue_front = 0;
	m_threadpool->queue_rear = 0;

	if(pthread_mutex_init(&m_threadpool->lock,NULL)!=0 || pthread_cond_init(&m_threadpool->not_full,NULL)!= 0 || pthread_cond_init(&m_threadpool->not_empty,NULL)!= 0)
	{
		cout << "THREAD CREATE INIT LOCK OR COND ERROR.." <<endl;
		return NULL;
	}
	for(int i=0;i<min;i++)
	{
		pthread_create(&m_threadpool->tids[i],NULL,this->thread_custom_job,(void*)m_threadpool);
		++m_threadpool->thread_alive;
	}
	pthread_create(&m_threadpool->manager_id,NULL,this->thread_manager_job,(void*)m_threadpool);
	return m_threadpool;
}

bool CThread_Pool::thread_pool_destroy()
{
	//destoy(m_threadpool);
	return 0;
}

bool CThread_Pool::thread_add_task(task_t task)
{
	pthread_mutex_lock(&m_threadpool->lock);
	if(m_threadpool->thread_shutdown)
	{
		if(m_threadpool->queue_size == m_threadpool->queue_max)
		{
			pthread_cond_wait(&m_threadpool->not_full,&m_threadpool->lock);
		}
		if(m_threadpool->thread_shutdown)
		{
			m_threadpool->queue_task[m_threadpool->queue_front].job = task.job;
			m_threadpool->queue_task[m_threadpool->queue_front].arg = task.arg;
			++m_threadpool->queue_size;
			m_threadpool->queue_front = (m_threadpool->queue_front + 1) % m_threadpool->queue_max;
			pthread_cond_signal(&m_threadpool->not_empty);
		}
	}
	pthread_mutex_unlock(&m_threadpool->lock);
	return 0;
}

void* CThread_Pool::thread_custom_job(void* arg)
{
	task_t task;
	m_threadpool = (pool_t*)arg;
	while(m_threadpool->thread_shutdown)
	{
		pthread_mutex_lock(&m_threadpool->lock);
		while(m_threadpool->queue_size == 0)
		{
			cout << "waiting.." << endl;
			pthread_cond_wait(&m_threadpool->not_empty,&m_threadpool->lock);
		}
		if(m_threadpool->thread_exit > 0)
		{
			--m_threadpool->thread_exit;
			--m_threadpool->thread_alive;
			pthread_mutex_unlock(&m_threadpool->lock);
			pthread_exit(NULL);
		}
		if(m_threadpool->thread_shutdown)
		{
			task.job = m_threadpool->queue_task[m_threadpool->queue_rear].job;
			task.arg = m_threadpool->queue_task[m_threadpool->queue_rear].arg;

			--m_threadpool->queue_size;
			pthread_cond_signal(&m_threadpool->not_full);
			++m_threadpool->thread_busy;

			pthread_mutex_unlock(&m_threadpool->lock);
			cout << "num of task_queue now:" << m_threadpool->queue_size << endl;
			(task.job)(task.arg);
			pthread_mutex_lock(&m_threadpool->lock);
			--m_threadpool->thread_busy;
		}
		else
		{
			--m_threadpool->thread_alive;
			pthread_mutex_unlock(&m_threadpool->lock);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&m_threadpool->lock);
	}
	return NULL;

}

void* CThread_Pool::thread_manager_job(void* arg)
{
	pool_t*p = (pool_t*)arg;
	int alive;
	int busy;
	int size;
	int add = 0;
	while(p->thread_shutdown)
	{
		pthread_mutex_lock(&p->lock);
		alive = p->thread_alive;
		busy = p->thread_busy;
		size = p->queue_size;
		pthread_mutex_unlock(&p->lock);
		if((size > alive-busy || busy/alive*(double)100 >= (double)70) && alive + 10 <= p->thread_max)
		{

			for(int i=0;i<p->thread_max&&add<10; i++)
			{
				if(p->tids[i] == 0 || !thread_if_alive(p->tids[i]))
				{
					pthread_create(&p->tids[i],NULL,this->thread_custom_job,(void*)p);
				}
			}
		}
		if(busy*2 < alive-busy && alive - 10 >= p->thread_min)
		{
			pthread_mutex_lock(&p->lock);
			p->thread_exit = 10;
			pthread_mutex_unlock(&p->lock);
			for(int i=0;i<10;i++)
			{
				pthread_cond_signal(&p->not_empty);
			}
		}
		sleep(2);
	}
	return NULL;
}

void* CThread_Pool::user_job(void* arg) 
{
	cout << "user job is running.." << endl;
	int clientfd = *(int *)arg;
	char buf[1500];
	int len = read(clientfd,buf,sizeof(buf));
	int j;
	j = 0;
	while(len > j)
	{
		buf[j] = toupper(buf[j]);
		j++;
	}
	write(clientfd,buf,len);

	bzero(buf,sizeof(buf));
	return NULL;

}

bool CThread_Pool::thread_if_alive(pthread_t tid)
{

	int err;
	if((err = pthread_kill(tid,0)) == 0)
	{
		return 1;
	}
	else
	{
		if(errno == ESRCH)
		{
			return 0;
		}
	}
	return 0;
}

bool CThread_Pool::thread_socket_init()
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
