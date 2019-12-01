#ifndef MYDAEMON_H
#define MYDAEMON_H

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;

class CMyDaemon
{
	public:
		CMyDaemon();
		~CMyDaemon();
	public:
		pid_t pid;
	public:
		bool CreateDaemon();
};
#endif
