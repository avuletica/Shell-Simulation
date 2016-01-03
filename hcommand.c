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
	int wstatus;
	int cstatus;    
		
	cpid=fork();
	if(cpid == 0) {
		if (strcmp(command, "kill") == 0)
			execlp(command, command, arguments, NULL);
		else {
			cstatus = execlp(command, command,NULL);
			if (cstatus && !strchr(command,'&'))
				printf("%s: command not found\n",command);
		}                   
				
		/* Keep running if it's background process */
		if(!strchr(command,'&'))
			_exit(0);       
	}
	else {
		cpid=waitpid(0, &wstatus, 0);
		/*
		* Handle exit status of process prototype
		*if (WIFEXITED(wstatus)) {
		*	printf("Exit status of CHILD process: %d\n", WEXITSTATUS(wstatus));
		*	fflush(stdout);
		*}
		*else if (WIFSIGNALED(wstatus)) {
		*	fprintf(stderr, "CHILD process interupted with signal: %d\n", WTERMSIG(wstatus));
		*	fflush(stderr);
		*}
		*/
	}
	return 0;
}