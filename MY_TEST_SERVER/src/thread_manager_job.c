#include <thread_pool.h>

void* thread_manager_job(void* arg)
{
	pool_t *p = (pool_t*)arg; 
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
		if((size > alive - busy || busy / alive * (double)100 >= (double)70) && alive + 10 <= p->thread_max)
		{
			for(int i=0;i<p->thread_max&&add<10;i++)
			{
				if(p->tids[i] == 0 || !thread_if_alive(p->tids[i]))
				{
					pthread_create(&p->tids[i],NULL,thread_custom_job,(void*)p);
				}
			}
		}
		if(busy * 2 < alive - busy && alive - 10 >= p->thread_min)
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
