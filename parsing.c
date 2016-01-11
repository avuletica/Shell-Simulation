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
	char argument[max_len] = {'\0'};
	char pipearg1[max_len] = {'\0'};
	char pipearg2[max_len] = {'\0'};
	char *ret;
	char *token;
	char *inptcpy;
	char word1[max_len] = {'\0'};
	char word2[max_len] = {'\0'};		
	int inptlen;
	int check=1;    

	/* Copy buffer for parsing */
	inptlen = strlen(buffer);
	inptcpy = (char*) calloc(inptlen + 1, sizeof(char));
	strncpy(inptcpy, buffer, inptlen);	

	/* Check for redirection (>) */
	ret = strchr(buffer, '>');
	if (ret) {
		//* word1 = left of pipe, word2 = right of pipe */
		token = strtok(buffer, ">");
		strcpy(word1, token);
	   	/* walk through other tokens */
	   	while( token != NULL ) {   
	      		token = strtok(NULL, ">");
	      		if(token)
				strcpy(word2, token);
	      	}		

		/* word1 = first command
		*  argument = first argument
		*/
		token = strtok(word1, " ");
		token = strtok(NULL, " ");
		if(token)
			strcpy(argument, token);

		remove_spaces(word1);
		remove_spaces(word2);
		if(strcmp(argument, " ") != 0)
			remove_spaces(argument);

		printf("%s %s %s\n",word1, argument, word2);
		redirection_output(word1, argument, word2);
		check = 0;        
	}

	/* Check for redirection (<) */
	ret = strchr(buffer, '<');
	if (ret) {
		/* 
		* Get the first token, copy it to word1,
		* get the second token, copy it to word2.
		*/
		token = strtok(buffer, "<");
		strcpy(word1, token);
		token = strtok(NULL, "<");
		strcpy(word2, token);
		remove_spaces(word1);
		remove_spaces(word2);
		redirection_input(word1, word2);
		check = 0;
	}

	/* Check for pipe (|) */
	ret = strchr(buffer, '|');
	if (ret) {
		/* word1 = left of pipe, word2 = right of pipe */
		token = strtok(buffer, "|");
		strcpy(word1, token);
	   	/* walk through other tokens */
	   	while( token != NULL ) {   
	      		token = strtok(NULL, "|");
	      		if(token)
				strcpy(word2, token);
	      	}
				
		/* word1 = first command
		*  pipearg1 = first argument
		*/
		token = strtok(word1, " ");
		token = strtok(NULL, " ");
		if(token)
			strcpy(pipearg1, token);

		/* word2 = second command
		*  pipearg2 = second argument
		*/
		token = strtok(word2, " ");
		token = strtok(NULL, " ");
		if(token)
			strcpy(pipearg2, token);
		
		/* Remove spaces to handle edge cases */
		remove_spaces(word1);
		if(strcmp(pipearg1, " ") != 0)
			remove_spaces(pipearg1);
		
		remove_spaces(word2);
		if(strcmp(pipearg2, " ") != 0)
			remove_spaces(pipearg2);

		handle_pipe(word1, pipearg1, word2, pipearg2);
		check = 0;      
	}        

	/* Isolate command */
	strcpy(command, buffer);
	strtok(command, " ");
	
	/* Isolate argument */   
	token = strtok(inptcpy, " ");
	token = strtok(NULL, " ");
	if (token)
		strcpy(argument, token);
 
	/* If there is no redirection or pipe, handle command. */
	if (check)
		handle_command(command, argument, buffer, ret);  
	
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