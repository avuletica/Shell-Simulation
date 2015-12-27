#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <pwd.h>

void current_working_directory()
{
	char cwd[512]={'\0'};
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		fprintf(stdout, "Current working dir: %s\n", cwd);
	else
		perror("getcwd() error");
}

int main(int argc, char **argv)
{
	char command[100]={'\0'};	
	struct passwd *p;
	char hostname[1024] = {'\0'};

	printf("----- C O N S O L E\t S I M U L A T I O N -----\n\n");
	while (1) {	
		/* Get & display username and hostname */
		passwd *p = getpwuid(getuid());
		gethostname(hostname, 1023);
		printf("[%s@%s]# ",p->pw_name,hostname);
		
		/* Get user input */		
		scanf(" %s",command);
		if (strcmp(command,"cwd") == 0) 
			current_working_directory();		
		else if(strcmp(command,"quit") == 0)
			return 0;
		else
			printf("%s: command not found\n",command);
	}
	return 0;
}
