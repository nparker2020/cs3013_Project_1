// ebruklich & nparker
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
	struct timeval beforeTime, afterTime;
	gettimeofday(&beforeTime, NULL);
	struct rusage globalUsage;

	char* fileName = "custom.txt";
	FILE* customFile = fopen(fileName, "r");
	
	if(!customFile) 
	{
		printf("There was an error opening %s...", fileName);	
		return -1;
	}
	char* line = NULL;
	int buffer_size = 0;
	int count = 0;
	//when line == NULL && buffer_size == 0, the getline method
	//will automatically allocate a buffer of the correct size.
	int length = 0;	
	while( (length = getline(&line, &buffer_size, customFile)) != -1) 
	{
		//parse through line and replace newlines with none character.		
		for(int i = 0; i < length; i++)
		{
			if(line[i] == '\n') 
			{
				line[i] = '\0';
			}
		}
	
		char* splitWord = 0;
		char* commandWord = NULL;		
		commandWord = strtok(line, " ");
		splitWord = commandWord;
		char * arguments[32];
		int count = 1;
		arguments[0] = commandWord;
		while( splitWord != NULL)
		{
			splitWord = strtok(NULL, " ");
			if(splitWord != NULL) 
			{
				arguments[count] = malloc(strlen(splitWord) + 1);
				strcpy(arguments[count], splitWord);
				count++;		
			}
		}

		printf("Running command: %s \n", line);
		if(!strcmp(commandWord, "ccd")) 
		{
			//printf("changing directory... [%s] \n", commandWord);
			chdir(arguments[1]);
			printf("Changed to directory: %s \n", arguments[1]);
		}else if(!strcmp(commandWord, "pwd")) 
		{
			//printf("running getcwd()...\n");
			char pathString[256];
			getcwd(pathString, sizeof(pathString));
			printf("Current working directory: %s \n", pathString);
			//print statics here?							
		}else
		{
			int rc = fork();
			if (rc < 0) 
			{
				printf("fork failed!\n");
			}else if(rc == 0) 
			{
				//child process created.		
			
		/*		printf("with arguments: \n");
				for(int i = 1; i < count; i++) 
				{
					printf("[%s]\n", arguments[i]);
				}*/
				arguments[count] = NULL;
				char* args[3];
				execvp(arguments[0], arguments);
			}
			else
			{
				//wait for child process to complete before printing stats
				int rc_wait = wait(NULL);
				gettimeofday(&afterTime, NULL);
				int start_time_value = beforeTime.tv_usec;
				int after_time_value = afterTime.tv_usec;
				int start_time_seconds = beforeTime.tv_sec;
				int after_time_seconds = afterTime.tv_sec;

				long difference = (after_time_value - start_time_value)/1000 + (after_time_seconds - start_time_seconds)*1000;
			
				getrusage(RUSAGE_CHILDREN, &globalUsage);
				long pgFaults = globalUsage.ru_majflt;
				long unpgs = globalUsage.ru_minflt;
				printf("\n");
				printf("-- Statistics --\n");
				printf("Elapsed time: %ld milliseconds\n", difference);
				printf("Page Faults: %ld\n", pgFaults);
				printf("Page Faults (reclaimed): %ld\n", unpgs); 
				printf("-- End of Statistics --\n");
				printf("\n"); 
			}
				
		}

		
	}

	return 0;
}
