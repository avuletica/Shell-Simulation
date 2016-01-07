#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include "dirutils.h"
#include "shutility.h"

void handle_command(char *command, char *arguments, char *buffer, char *ret)
{
	/*
	* handle_ custom commands,
	* using kill to end all child processes @ exit,
	* else execute shell(bash) builtin commands.
	*/
	if (strcmp(command,"q") == 0 || strcmp(command,"quit") == 0)
		kill(0, SIGINT);
	else if (strcmp(command, "cd") == 0)
		change_directory(buffer);
	else if (strcmp(command, "cwd") == 0) 
		current_working_directory();
	else if (!ret)
		execute_command(command, arguments); 
}

int execute_command(char *command,char *arguments)
{
	int cpid;
	int rtrnstatus;
	int cstatus;    
	
	cpid=fork();
	if(cpid == 0) {
		if (strcmp(command, "kill") == 0)
			execlp(command, command, arguments, NULL);
		else if (strcmp(command, "rm") == 0)
			unlink(arguments);
		else {
			/* If we have arguments, execute with arguments... */
			if (strcmp(arguments, "\0") != 0)
				cstatus = execlp(command, command, arguments, NULL);
			else
				cstatus = execlp(command, command, NULL);
			if (cstatus && !strchr(command,'&'))
				printf("%s: command not found\n",command);
		}                   
				
		/* Keep running if it's background process */
		if(!strchr(command,'&'))
			_exit(0);       
	}
	else {
		waitpid(cpid, &rtrnstatus, 0);
		/*
		if (WIFEXITED(rtrnstatus))
			printf("Child's exit code %d\n", WEXITSTATUS(rtrnstatus));
		else
			printf("Child did not terminate with exit\n");
		*/
	}
	return 0;
}