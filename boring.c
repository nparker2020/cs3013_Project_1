#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>


int main(int argc, char *argv[]) 
{
	struct timeval beforeTime, afterTime;
	gettimeofday(&beforeTime, NULL);

	int rc = fork();
	if (rc < 0) 
	{
		printf("fork failed!\n");
	}else if(rc == 0) 
	{
		//child process created.		
		printf("Child Process Created!\n");
		char * args[2];
		args[0] = "whoami";
		args[1] = NULL;
		execvp(args[0], args);
	}
	else
	{
		int rc_wait = wait(NULL);
		gettimeofday(&afterTime, NULL);
		int start_time_value = beforeTime.tv_usec;
		int after_time_value = afterTime.tv_usec;
		int difference = after_time_value - start_time_value;
		difference = difference/1000;
		printf("-- Statistics --\n");
		printf("Elapsed time: %d milliseconds\n", difference);
		printf("Parent Process Completed.\n");
	}
	
	return 0;
}
