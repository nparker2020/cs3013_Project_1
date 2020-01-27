// ebruklich & nparker
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char *argv[]) 
{
	char* fileName = "multi.txt";
	FILE* customFile = fopen(fileName, "r");
	
	if(!customFile) 
	{
		printf("There was an error opening %s...", fileName);	
		return -1;
	}
	char* line = NULL;
	int buffer_size = 0;
	//when line == NULL && buffer_size == 0, the getline method
	//will automatically allocate a buffer of the correct size.
	while( getline(&line, &buffer_size, customFile) != -1) 
	{
		printf("Read line: %s \n", line);	
	}

	return 0;
}
