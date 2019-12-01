#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include "sys.h"


class CThread_Pool{
	public:
		CThread_Pool();
		~CThread_Pool();
	public:
		pool_t *m_threadpool;
	public:
		/** 
		 * 创建线程池
		 * @param 最小线程数
		 * @param 最大线程数
		 * @param 最大任务队列数
		*/ 
		pool_t* thread_pool_create(int,int,int);
		/**
		 * 功能：销毁线程池
		*/
		bool thread_pool_destroy();
		/**
		 * 向任务队列添加任务
		 * @param 任务	
		*/
		bool thread_add_task(task_t);
		/**
		 * 消费者任务
		*/
		void* thread_custom_job(void*);
		/**
		 * 管理者任务
		*/
		void* thread_manager_job(void*);
		/**
		 * 用户任务
		*/
		void* user_job(void*);
		/**
		 * 判断线程是否存活
		*/
		bool thread_if_alive(pthread_t);
		/**
		 * 线程socket初始化
		*/
		bool thread_socket_init(void);

};

#endif
