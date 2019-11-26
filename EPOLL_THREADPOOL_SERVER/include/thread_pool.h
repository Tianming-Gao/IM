#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>

#define EPOLL_MAX 1000

typedef struct 
{
	void* (*job)(void*);
	void* arg;
}task_t;

typedef struct 
{
	int thread_max;
	int thread_min;
	int thread_alive;
	int thread_busy;
	int thread_exit;
	int thread_shutdown;
	pthread_t *tids;
	pthread_t manager_id;
	task_t* queue_task;
	int queue_max;
	int queue_size;
	int queue_front;
	int queue_rear;

	pthread_cond_t not_full;
	pthread_cond_t not_empty;
	pthread_mutex_t lock;
}pool_t;

pool_t* thread_pool_create(int,int,int);
int thread_pool_destroy(pool_t*);
int thread_add_task(pool_t*,task_t);
void* thread_custom_job(void*);
void* thread_manager_job(void*);
void* user_job(void*);
int thread_if_alive(pthread_t);
int thread_socket_init(void);
