#ifndef SYS_H
#define SYS_H

#include <iostream>

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

using namespace std;
#define EPOLL_MAX 1000

//任务类型结构体
typedef struct 
{
	void* (*job)(void*);   //函数指针
	void* arg;             //函数传参
}task_t;

//线程池类型结构体
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


#endif
