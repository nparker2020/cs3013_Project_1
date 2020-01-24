//Eva Bruklich & Noah Parker, Boring Commander
// CS3013 -- Written C20
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char *argv[]) 
{
	struct timeval beforeTime, afterTime;
	gettimeofday(&beforeTime, NULL);
	struct rusage globalUsage; 
	int rc = fork();
	if (rc < 0) 
	{
		printf("fork failed!\n");
	}else if(rc == 0) 
	{
		//child process created.		
		printf("Running command: whoami\n");
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
		
		getrusage(RUSAGE_CHILDREN, &globalUsage);
		long pgFaults = globalUsage.ru_majflt;
		long unPgs = globalUsage.ru_minflt;
		printf("\n");
		printf("-- Statistics --\n");
		printf("Elapsed time: %d milliseconds\n", difference);
		printf("Page Faults: %ld\n", pgFaults);
		printf("Page Faults (reclaimed): %ld\n", unPgs);
		printf("-- End of Statistics --\n");
		printf("\n");
	}

	rc = fork();
	if (rc < 0) 
	{
		printf("fork failed!\n");
	}else if(rc == 0) 
	{
		//child process created.		
		printf("Running command: last\n");
		char * args[2];
		args[0] = "last";
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
		
		getrusage(RUSAGE_CHILDREN, &globalUsage);
		long pgFaults = globalUsage.ru_majflt;
		long unPgs = globalUsage.ru_minflt;
		printf("\n");
		printf("-- Statistics --\n");
		printf("Elapsed time: %d milliseconds\n", difference);
		printf("Page Faults: %ld\n", pgFaults);
		printf("Page Faults (reclaimed): %ld\n", unPgs);
		printf("-- End of Statistics --\n");
		printf("\n");
	}

	rc = fork();
	if (rc < 0) 
	{
		printf("fork failed!\n");
	}else if(rc == 0) 
	{
		//child process created.		
		printf("Running command: ls -al /home\n");
		char * args[4];
		args[0] = "ls";
		args[1] = "-al";
		args[2] = "/home";
		args[3] = NULL;
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
		
		getrusage(RUSAGE_CHILDREN, &globalUsage);
		long pgFaults = globalUsage.ru_majflt;
		long unPgs = globalUsage.ru_minflt;
		printf("\n");
		printf("-- Statistics --\n");
		printf("Elapsed time: %d milliseconds\n", difference);
		printf("Page Faults: %ld\n", pgFaults);
		printf("Page Faults (reclaimed): %ld\n", unPgs);
		printf("-- End of Statistics --\n");
		printf("\n");
	}
	
	return 0;
}
