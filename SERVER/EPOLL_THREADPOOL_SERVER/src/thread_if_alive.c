#include <thread_pool.h>

int thread_if_alive(pthread_t tid)
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
