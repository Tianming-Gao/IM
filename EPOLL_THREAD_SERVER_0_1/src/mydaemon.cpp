#include "mydaemon.h"

CMyDaemon::CMyDaemon()
{
}

CMyDaemon::~CMyDaemon()
{
}

bool CMyDaemon::CreateDaemon()
{
	pid = fork();
	if(pid > 0)
	{
		exit(0);
	}
	else if(pid == 0)
	{
		setsid();
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
		chdir("/tmp/");
		umask(2);
		//daemon job here
		while(1)
		{
			sleep(100);
		}
	}
	else
	{
		chdir("/tmp/");
		umask(2);
		int fd = open("MyDaemon_Job_errortime.log",O_RDWR|O_CREAT,0664);
		time_t t;
		char timebuf[100];
		t = time(NULL);
		ctime_r(&t,timebuf);
		write(fd,timebuf,strlen(timebuf));
		bzero(timebuf,sizeof(timebuf));
		close(fd);
	}
	return 0;
}




