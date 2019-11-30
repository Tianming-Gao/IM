#include <thread_pool.h>

pool_t* thread_pool_create(int min,int max,int quemax)
{
	pool_t *p = (pool_t *)malloc(sizeof(pool_t));

	p->thread_max = max;
	p->thread_min = min;
	p->queue_max = quemax;

	p->thread_alive = 0;
	p->thread_busy = 0;
	p->thread_shutdown = 1;

	p->tids = (pthread_t*)malloc(max * sizeof(pthread_t));
	bzero(p->tids,max*sizeof(pthread_t));
	p->queue_task = (task_t*)malloc(quemax * sizeof(task_t));
	p->queue_size = 0;
	p->queue_front = 0;
	p->queue_rear = 0;

	if(pthread_mutex_init(&p->lock,NULL)!=0 || pthread_cond_init(&p->not_full,NULL)!=0 || pthread_cond_init(&p->not_empty,NULL)!=0)
	{
		printf("THREAD CREATE INIT LOCK OR COND ERROR..\n");
		return NULL;
	}

	//create min thread
	for(int i=0;i<min;i++)
	{
		pthread_create(&p->tids[i],NULL,thread_custom_job,(void*)p);
		++p->thread_alive;
	}
	pthread_create(&p->manager_id,NULL,thread_manager_job,(void*)p);

	return p;
}
