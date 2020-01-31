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

	struct processInfo 
	{
		int pid;
		struct timeval beforeTime;
		char* command;
	};

	char* fileName = "multi.txt";
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
	
	int * lineNumbers = malloc(sizeof(int)*argc);
	
	for(int i = 1; i < argc; i++) 
	{
		int value = atoi(argv[i]);
		lineNumbers[i] = value;
		//printf("lineNumbers[%d]: %d\n", i, lineNumbers[i]);	
	}

	char backgroundCommands[32][128];
	// 32 array of processID
	struct processInfo backgroundProcesses[32];

	int backgroundCount = 0;
	int runningBackgroundProcesses = 0;
	int currentLineNumber = 1;
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
		
		int background = 0; //false
		for(int i = 0; i < argc; i++) 
		{
			if(lineNumbers[i] == currentLineNumber) 
			{
				//We are going to run this one in the background.			
				background = 1;			
			}
		}
		printf("Running command: %s \n", line);
		if(background == 1) 
		{
						
			strcpy(backgroundCommands[backgroundCount], line);
			backgroundProcesses[backgroundCount].command = backgroundCommands[backgroundCount];
			
			//backgroundCommands[backgroundCount] = *commandValue;
			
			runningBackgroundProcesses++;
			printf("Background: ID [%d]: %s \n", backgroundCount, line);
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
		}else if(!strcmp(commandWord, "cproclist")) 
		{
			printf("-- Background Processes --\n");
			for(int i = 0; i < backgroundCount; i++) 
			{
				printf("[%d] %s \n", i, backgroundCommands[i]); 
			}
		}
		else
		{
			gettimeofday(&beforeTime, NULL);
			gettimeofday(&backgroundProcesses[backgroundCount].beforeTime, NULL);
			struct timeval nonBackgroundBefore;
			int rc = fork();
			if (rc < 0) 
			{
				printf("fork failed!\n");
			}else if(rc == 0) 
			{
				
				arguments[count] = NULL;
				char* args[3];
				execvp(arguments[0], arguments);
			}
			else
			{
				//wait for child process to complete before printing stats
				//here, rc == the child's process ID				
				if(background == 0) 
				{
					int rc_wait = wait(NULL);

					gettimeofday(&afterTime, NULL);
					int start_time_value = beforeTime.tv_usec;
					int after_time_value = afterTime.tv_usec;
					int diff = after_time_value - start_time_value; 
					diff = diff/1000;
				
					getrusage(RUSAGE_CHILDREN, &globalUsage);
					long pgFaults = globalUsage.ru_majflt;
					long unpgs = globalUsage.ru_minflt;
					printf("\n");
					printf("-- Statistics --\n");
					printf("Elapsed time: %d milliseconds\n", diff);
					printf("Page Faults: %ld\n", pgFaults);
					printf("Page Faults (reclaimed): %ld\n", unpgs); 
					printf("-- End of Statistics --\n");
					printf("\n");				
				}else{
					//printf("running process in the background. \n");
					struct rusage backgroundUsage;	
					backgroundProcesses[backgroundCount].pid = rc;		
					backgroundCount++;
					while(1) 
					{
			
						int result = wait3(NULL, WNOHANG, &backgroundUsage);
						if(result == -1 || result == 0) 
						{
							break;
						}else
						{
							//backgroundCount--;
							//figure out which command finished and remove it from the background commands array							
							runningBackgroundProcesses--;
							long pgFaults = backgroundUsage.ru_majflt;
							long unpgs = backgroundUsage.ru_minflt;
							struct timeval afterTime;
							struct timeval beforeTime;
							for(int i = 0; i < backgroundCount; i++)
							{
								if(backgroundProcesses[i].pid == result) 
								{
									printf("-- Job Complete [%d: %s] --\n", backgroundProcesses[i].pid, backgroundProcesses[i].command);
								beforeTime = backgroundProcesses[i].beforeTime;								
								}
							}
							gettimeofday(&afterTime, NULL); 
							 
							int start_time_value = beforeTime.tv_usec;
							int after_time_value = afterTime.tv_usec;
							int diff = after_time_value - start_time_value; 
							diff = diff/1000;

							//printf("-- Job Complete [%d: %s] --\n", 0, commandLine);
							printf("Process ID: %d", result);
							printf("\n");
							printf("-- Statistics --\n");
							printf("Elapsed time: %d milliseconds\n", diff);
							printf("Page Faults: %ld\n", pgFaults);
							printf("Page Faults (reclaimed): %ld\n", unpgs); 
							printf("-- End of Statistics --\n");
							printf("\n");				
						}				
					}			
				}
				 
			}
				
		}
		
		currentLineNumber++;
		
	}

	struct rusage backgroundUsage;		
	while(1) 
	{
			
		int result = wait3(NULL, WNOHANG, &backgroundUsage);
		if(runningBackgroundProcesses <=0) 
		{
			break;
		}else
		{
			if(result > 0) 
			{
			
		
			runningBackgroundProcesses--;
			//figure out which command finished and remove it from the background commands array							
			long pgFaults = backgroundUsage.ru_majflt;
			long unpgs = backgroundUsage.ru_minflt;
			struct timeval afterTime;
			struct timeval beforeTime;
			for(int i = 0; i < backgroundCount; i++)
			{
				if(backgroundProcesses[i].pid == result) 
				{
					printf("-- Job Complete [%d: %s] --\n", backgroundProcesses[i].pid, backgroundProcesses[i].command);
					beforeTime = backgroundProcesses[i].beforeTime;								
				}
			}
			gettimeofday(&afterTime, NULL);

			int start_time_value = beforeTime.tv_usec;
			int after_time_value = afterTime.tv_usec;
			int diff = after_time_value - start_time_value; 
			diff = diff/1000;
			//printf("-- Job Complete [%d: %s] --\n", 0, commandLine);
			printf("Process ID: %d", result);
			printf("\n");
			printf("-- Statistics --\n");
			printf("Elapsed time: %d milliseconds\n", diff);
			printf("Page Faults: %ld\n", pgFaults);
			printf("Page Faults (reclaimed): %ld\n", unpgs); 
			printf("-- End of Statistics --\n");
			printf("\n");			





			}		
				
		}				
	}			


	return 0;
}
