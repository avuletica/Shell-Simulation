#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shutility.h"
#include "parsing.h"
#include "hcommand.h"

#define max_len 20

int parsing(char *buffer)
{
	char command[max_len] = {'\0'};
	char arguments[max_len] = {'\0'};
	char *ret;
	char *token;
	char *inptcpy;
	char word1[max_len] = {'\0'};
	char word2[max_len] = {'\0'};
	const char ch1[2] = ">";
	const char ch2[2] = "<";
	const char ch3[2] = "|";
	const char ch4[2] = " ";	
	int inptlen;
	int check=1;    

	/* Copy buffer for parsing */
	inptlen = strlen(buffer);
	inptcpy = (char*) calloc(inptlen + 1, sizeof(char));
	strncpy(inptcpy, buffer, inptlen);	

	/* Check for redirection (>) */
	ret = strchr(buffer, '>');
	if (ret) {
		/* 
		* Get the first token, copy it to word1,
		* get the second token, copy it to word2.
		*/
		token = strtok(buffer, ch1);
		strcpy(word1, token);
		token = strtok(NULL, ch1);
		strcpy(word2, token);
		remove_spaces(word1);
		remove_spaces(word2);
		redirection_output(word1, word2);
		check = 0;        
	}

	/* Check for redirection (<) */
	ret = strchr(buffer, '<');
	if (ret) {
		/* 
		* Get the first token, copy it to word1,
		* get the second token, copy it to word2.
		*/
		token = strtok(buffer, ch2);
		strcpy(word1, token);
		token = strtok(NULL, ch2);
		strcpy(word2, token);
		remove_spaces(word1);
		remove_spaces(word2);
		redirection_input(word1, word2);
		check = 0;
	}

	/* Check for pipe (|) */
	ret = strchr(buffer, '|');
	if (ret) {
		token = strtok(buffer, ch3);
		strcpy(word1, token);
		token = strtok(NULL, ch3);
		strcpy(word2, token);
		puts("Pipe detected, handling...");
		remove_spaces(word1);
		remove_spaces(word2);
		handle_pipe(word1, word2); 
		check = 0;      
	}        

	/* Isolate command */
	strcpy(command, buffer);
	strtok(command, " ");
	
	/* Isolate arguments */   
	token = strtok(inptcpy, ch4);
	token = strtok(NULL, ch4);
	if (token)
		strcpy(arguments, token);
 
	/* If there is no redirection or pipe, handle command. */
	if (check)
		handle_command(command, arguments, buffer, ret);  
	
	check = 1;

	if (inptcpy)
		free(inptcpy);       

	return 1;   
}

void remove_spaces(char* source)
{
	char *i = source;
	char *j = source;

	while (*j != 0) {
		*i = *j++;
		if (*i != ' ')
			i++;
	}
	*i = 0; 
}