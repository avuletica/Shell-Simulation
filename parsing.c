#include <stdio.h>
#include <string.h>
#include "dirutils.h"

int parsing(char *buffer)
{
	char command[20]={'\0'};

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

	return 1;
}