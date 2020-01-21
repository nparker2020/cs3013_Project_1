#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) 
{
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
		printf("Parent Process Completed.\n");
	}
	



	return 0;
}
