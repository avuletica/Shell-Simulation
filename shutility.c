#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>
#include "dirutils.h"

#define max_hname 128
#define max_path 256
#define max_cwd 256
#define READ_END 0
#define WRITE_END 1

void display_info()
{
    struct passwd *p;
    char hostname[max_hname] = {'\0'};
    char cwd[max_cwd]={'\0'};
    char *token;
    const char delimiter[2] = "/";  

    getcwd(cwd, sizeof(cwd));
    /* get the first token */
    token = strtok(cwd, delimiter);
    /* walk through other tokens */
    while ( token != NULL ) {
        token = strtok(NULL, delimiter);
        if (token != NULL)
            strcpy(cwd,token);
    }
    p = getpwuid(getuid());
    gethostname(hostname, max_hname);
    printf("[%s@%s %s]# ",p->pw_name,hostname,cwd);
}

void redirection_input(char *word1, char *word2)
{
    int input;
    int chk;
    char c='0';

    input = open(word2, O_RDONLY, S_IRUSR | S_IWUSR);
    if (-1 == input) {
        perror("Opening file error!");
        return;
    }       
    if (-1 == dup2(input, fileno(stdin)))
        perror("Cannot redirect stdin!");

    while ((chk=read(STDIN_FILENO, &c, 1)) > 0)
        write(STDOUT_FILENO, &c, 1);

    puts("\n");
    close(input);
}

void redirection_output(char *word1, char *word2)
{
    int out;

    out = open(word2, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (-1 == out)
        perror("Opening file error!");
    if (-1 == dup2(out, fileno(stdout)))
        perror("Cannot redirect stdout!");

    fflush(stdout);
    close(out);
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
        /* Maybe in future 
        if (WIFEXITED(wstatus)) {
            printf("Exit status of CHILD process: %d\n", WEXITSTATUS(wstatus));
            fflush(stdout);
        }
        else if (WIFSIGNALED(wstatus)) {
            fprintf(stderr, "CHILD process interupted with signal: %d\n", WTERMSIG(wstatus));
            fflush(stderr);
        }
        */
    }
    return 0;
}

void handle_pipe(char *word1, char *word2)
{
    int pipefd[2];
    int pid1, pid2;
    int status;

    /* Create Pipe */
    if (pipe(pipefd) == -1) {
        perror("pipe");
        _exit(0);
    }
    
    /* First fork */
    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        _exit(0);
    }
    /* CHILD 1 */
    if (pid1 == 0) {               
        if (dup2(pipefd[WRITE_END], STDOUT_FILENO) == -1) {
            perror("dup2");
            _exit(0);
        }
        close(pipefd[READ_END]);            /* Don't need pipe READ_END */
        if (pipefd[WRITE_END] != STDOUT_FILENO)
            close(pipefd[WRITE_END]);    
        execlp(word1, word1, NULL);
    }    
    close(pipefd[WRITE_END]);

    /* Second fork */
    pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        kill(pid1, SIGTERM);
        _exit(0);
    }
    /* CHILD 2 */
    if (pid2 == 0) {           
        if (dup2(pipefd[READ_END], STDIN_FILENO) == -1) {
            perror("dup2");
            /* Kill first process */
            kill(pid1, SIGTERM);
            _exit(0);
        }
        if (pipefd[READ_END] != STDIN_FILENO)
            close(pipefd[READ_END]);        
        execlp(word2, word2, NULL);
    }    
    close (pipefd[READ_END]); 

    /* Wait for childs */
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);
}

