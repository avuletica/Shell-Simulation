#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "dirutils.h"
#include "shutility.h"

#define max_len 20

int parsing(char *buffer)
{
	char command[max_len]={'\0'};
	char *ret;
	char *token;
	char word1[max_len]={'\0'};
	char word2[max_len]={'\0'};
	const char ch1[2] = ">";
	const char ch2[2] = "<";
	int save_out;
	int save_inpt;

	/* Save stdout/stdin for restoring purposes */
	save_out = dup(fileno(stdout));
	save_inpt = dup(fileno(stdin));
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
		remove_spaces(word2);
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
		remove_spaces(word2);
		redirection_input(word1,word2);
	}
	/* Isolate command */
	strcpy(command,buffer);
	strtok(command," ");
	/* Base case */
	if (strcmp(command,"q") == 0)
		return 0;
	/* Handle command */
	handle_command(command,buffer);
	/* Restore stdin/stdout */
	dup2(save_out, fileno(stdout));
	dup2(save_inpt, fileno(stdin));
	close(save_out);
	close(save_inpt);		

	return 1;	
}