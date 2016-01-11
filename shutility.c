#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>

#define max_hname 128
#define max_path 256
#define max_cwd 256
#define READ_END 0
#define WRITE_END 1

void display_info()
{
        struct passwd *p;
        char hostname[max_hname] = {'\0'};
        char cwd[max_cwd] = {'\0'};
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
        printf("[%s@%s %s]# ",p->pw_name, hostname, cwd);
}

void redirection_input(char *word1, char *word2)
{
        pid_t cpid;
        int rtrnstatus;

        cpid = fork();

        if (cpid < 0) {
                perror("fork failed");
                return;
        }
        else if (cpid == 0) {
                execlp(word1, word1, word2, NULL);
        }
        else {
                /* Wait for child process to finish */
                waitpid(cpid, &rtrnstatus, 0);

                if (WIFEXITED(rtrnstatus))
                        printf("Child's exit code %d\n", WEXITSTATUS(rtrnstatus));
                else
                        printf("Child did not terminate with exit\n"); 
        }
}

void redirection_output(char *word1, char *argument, char *word2)
{    
        pid_t cpid;
        int rtrnstatus;
        int outpt;
        int save_out;

        outpt = open(word2, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (-1 == outpt) {
                perror(word2);
                return;
        }

        /* Save standard output */
        save_out = dup(fileno(stdout));
   
        if (-1 == dup2(outpt, fileno(stdout))) {
                perror("Cannot redirect stdout");
                return;
        }   

        cpid=fork();

        if (cpid < 0 ) {
                perror("Fork failed");
                return;
        }           
        else if (cpid == 0) {
        	if (strcmp(argument, "\0") == 0)
        		execlp(word1, word1, NULL);
        	else
        		execlp(word1, word1, argument, NULL);
        } 
                          
        else {
                /* Wait for child process to finish */
                waitpid(cpid, &rtrnstatus, 0);

                /* Restore standard output */
                dup2(save_out, fileno(stdout));
                close(save_out);
                close(outpt);

                if (WIFEXITED(rtrnstatus))
                        printf("Child's exit code %d\n", WEXITSTATUS(rtrnstatus));
                else
                        printf("Child did not terminate with exit\n");          
        }   
}

void handle_pipe(char *word1, char *pipearg1, char *word2, char *pipearg2)
{
        int pipefd[2];
        int status;
        pid_t pid1, pid2;

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
		/* Don't need pipe READ_END */
		close(pipefd[READ_END]);           
		if (pipefd[WRITE_END] != STDOUT_FILENO)
			close(pipefd[WRITE_END]);

		/* Execute depending on argument */
		if(strcmp(pipearg1, "\0") == 0)
			execlp(word1, word1, NULL);
		else
			execlp(word1, word1, pipearg1, NULL);
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

		/* Execute depending on argument */
		if(strcmp(pipearg2, "\0") == 0)
			execlp(word2, word2, NULL);
		else
			execlp(word2, word2, pipearg2, NULL);
	}    
	close (pipefd[READ_END]); 

	/* Wait for childs */
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
}

