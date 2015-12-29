#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "dirutils.h"
#include "shutility.h"

int parsing(char *buffer)
{
	char command[20]={'\0'};
	char *ret;
	char *token;
	char word1[20]={'\0'};
	char word2[20]={'\0'};
	const char ch1[2] = ">";
	const char ch2[2] = "<";
	int save_out;

	save_out = dup(fileno(stdout));

	/* Check for redirection (>) */
	ret = strchr(buffer,'>');
	if(ret) {
		/* 
		* Get the first token, copy it to word1,
		* get the second token, copy it to word2.
		*/
		token = strtok(buffer, ch1);
		strcpy(word1,token);
		token = strtok(NULL, ch1);
		strcpy(word2,token);
		redirection_output(word1,word2);		
	}

	/* Check for redirection (<) */
	ret = strchr(buffer, '<');
	if(ret) {
		/* 
		* Get the first token, copy it to word1,
		* get the second token, copy it to word2.
		*/
		token = strtok(buffer, ch2);
		strcpy(word1,token);
		token = strtok(NULL, ch2);
		strcpy(word2,token);
		redirection_input(word1,word2);
	}

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

	/* Reset stdin */
	dup2(save_out, fileno(stdout));
    close(save_out);

	return 1;
}