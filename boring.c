#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


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
	}
	else
	{
		printf("Parent Process Completed.\n");
	}
	
	return 0;
}
