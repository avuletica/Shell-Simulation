#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "dirutils.h"
#include "shutility.h"
#include "parsing.h"

#define max_len 20

int parsing(char *buffer)
{
    char command[max_len]={'\0'};
    char arguments[max_len]={'\0'};
    char *ret;
    char *token;
    char word1[max_len]={'\0'};
    char word2[max_len]={'\0'};
    const char ch1[2] = ">";
    const char ch2[2] = "<";
    const char ch3[2] = "|";
    const char ch4[2] = " ";
    int save_out;
    int save_inpt;

    /* Save stdout/stdin for restoring purposes */
    save_out = dup(fileno(stdout));
    save_inpt = dup(fileno(stdin));

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
        remove_spaces(word2);
        redirection_output(word1, word2);        
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
        remove_spaces(word2);
        redirection_input(word1, word2);
    }

    /* Check for pipe (|) */
    ret = strchr(buffer, '|');
    if (ret) {
        token = strtok(buffer, ch3);
        strcpy(word1, token);
        token = strtok(NULL, ch3);
        strcpy(word2, token);
        puts("Pipe detected, handling...!");
        remove_spaces(word1);
        remove_spaces(word2);
        handle_pipe(word1, word2);       
    }        

    /* Isolate command */
    strcpy(command, buffer);
    strtok(command, " ");
    
    /* Isolate arguments */    
    token = strtok(buffer, ch4);
    token = strtok(NULL, ch4);
    if(token)
        strcpy(arguments, token);
    
    /*
    * handle_ custom commands,
    * using kill to end all child processes @ exit,
    * else execute shell(bash) builtin commands.
    */
    if (strcmp(command,"q") == 0)
        kill(0, SIGINT);
    else if (strcmp(command, "cd") == 0)
        change_directory(buffer);
    else if (strcmp(command, "cwd") == 0) 
        current_working_directory();
    else if(!ret)
        execute_command(command, arguments);   
    
    /* Restore stdin/stdout */
    dup2(save_out, fileno(stdout));
    dup2(save_inpt, fileno(stdin));
    close(save_out);
    close(save_inpt);       

    return 1;   
}

void remove_spaces(char* source)
{
  char* i = source;
  char* j = source;

    while (*j != 0) {
        *i = *j++;
        if(*i != ' ')
            i++;
    }
    *i = 0; 
}