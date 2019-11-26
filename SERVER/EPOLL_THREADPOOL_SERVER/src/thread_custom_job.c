#include <thread_pool.h>

void* thread_custom_job(void* arg)
{
	task_t task;
	pool_t *p = (pool_t*)arg;
	while(p->thread_shutdown)
	{
		pthread_mutex_lock(&p->lock);
		while(p->queue_size == 0)
		{
			printf("waiting..\n");
			pthread_cond_wait(&p->not_empty,&p->lock);
		}
		if(p->thread_exit > 0)
		{
			--p->thread_exit;
			--p->thread_alive;
			pthread_mutex_unlock(&p->lock);
			pthread_exit(NULL);
		}
		if(p->thread_shutdown)
		{
			task.job = p->queue_task[p->queue_rear].job;
			task.arg = p->queue_task[p->queue_rear].arg;

			--p->queue_size;
			pthread_cond_signal(&p->not_full);
			++p->thread_busy;

			pthread_mutex_unlock(&p->lock);
			printf("num of task_queue now:%d\n",p->queue_size);
			(task.job)(task.arg);
			pthread_mutex_lock(&p->lock);
			--p->thread_busy;
		}
		else
		{
			--p->thread_alive;
			pthread_mutex_unlock(&p->lock);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&p->lock);
	}
	return NULL;
}

