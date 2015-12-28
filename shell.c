#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <pwd.h>
#include <dirent.h> 

void current_working_directory()
{
	char cwd[512]={'\0'};
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		fprintf(stdout, "Current working dir: %s\n", cwd);
	else
		perror("getcwd() error");
}
void change_directory(char *buffer)
{
	char delimiter[] = " ";
	char *command, *path;
	int inputLength;
	char *inputCopy;

	/* Copy buffer for parsing */
	inputLength = strlen(buffer);
	inputCopy = (char*) calloc(inputLength + 1, sizeof(char));
	strncpy(inputCopy, buffer, inputLength);
	/* Get first word (command) and path */
	command = strtok (inputCopy, delimiter);
	path = strtok (NULL, delimiter);

	if (strcmp(buffer,"cd ..") == 0)
		chdir("../");
	else if (strcmp(buffer,"cd") == 0 || strcmp(buffer,"cd ~") == 0)
		chdir(getenv("HOME"));
	else
		chdir(path);

	if(inputCopy)
		free(inputCopy);
}

void list_directory()
{
	DIR  *d;
	struct dirent *dir;

	d = opendir(".");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)		
			printf("%s\n", dir->d_name);
		
	closedir(d);
	}
}

int main(int argc, char **argv)
{
	char command[100]={'\0'};
	char hostname[100] = {'\0'};	
	char *buffer = NULL;
	struct passwd *p;	
    int read;
    size_t len;

	printf("----- C O N S O L E\t S I M U L A T I O N -----\n\n");
	while (1) {	
		/* Get & display username and hostname */
		passwd *p = getpwuid(getuid());
		gethostname(hostname, 99);
		printf("[%s@%s]# ",p->pw_name,hostname);
		
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
