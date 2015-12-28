#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <pwd.h>
#include <dirent.h> 
#include "dirutils.h"

void display_info()
{
	char hostname[100] = {'\0'};
	struct passwd *p;
	
	p = getpwuid(getuid());
	gethostname(hostname, 99);
	printf("[%s@%s]# ",p->pw_name,hostname);
}

int main(int argc, char **argv)
{
	char command[100]={'\0'};		
	char *buffer = NULL;	
    int read;
    size_t len;

	printf("----- C O N S O L E\t S I M U L A T I O N -----\n\n");
	while (1) {	
		/* Get & display username and hostname */
		display_info();
		/* Get user input, strtok to remove newline */		
		read = getline(&buffer, &len, stdin);
		strtok(buffer, "\n");
		/* Isolate command */
		strcpy(command,buffer);
		strtok(command," ");
	
		if (strcmp(command,"cwd") == 0) 
			current_working_directory();		
		else if(strcmp(command,"cd") == 0)
			change_directory(buffer);
		else if(strcmp(command,"ls") == 0)
			list_directory();
		else if(strcmp(command,"q") == 0)
			return 0;
		else
			printf("%s: command not found\n",command);
	}
	if(read)
    	free(buffer);

	return 0;
}
