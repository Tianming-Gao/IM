#include <thread_pool.h>
int Create_Daemon()
{
	pid_t pid;
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
		//todo
		//int fd = open("file.log",O_RDWR|O_CREAT,0664);
		//time_t t;
		//char timebuf[100];
		//while(1)
		//{
		//	t = time(NULL);
		//	ctime_r(&t,timebuf);
		//	write(fd,timebuf,strlen(timebuf));
		//	sleep(3);
		//	bzero(timebuf,sizeof(timebuf));
		//}
		//close
		while(1)
		{	
			Daemon_job();
		}		
		//		close(fd);
	}
	else
	{
		//error
		perror("create daemon error:");
	}
	return 0;
}
