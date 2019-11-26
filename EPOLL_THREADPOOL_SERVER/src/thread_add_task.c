#include <thread_pool.h>

int thread_add_task(pool_t *p,task_t task)
{
	pthread_mutex_lock(&p->lock);
	if(p->thread_shutdown)
	{
		if(p->queue_size == p->queue_max)
		{
			pthread_cond_wait(&p->not_full,&p->lock);
		}
		if(p->thread_shutdown)
		{
			p->queue_task[p->queue_front].job = task.job;
			p->queue_task[p->queue_front].arg = task.arg;
			++p->queue_size;
			p->queue_front = (p->queue_front + 1) % p->queue_max;
			pthread_cond_signal(&p->not_empty);
		}
	}
	pthread_mutex_unlock(&p->lock);
	return 0;
}
	
