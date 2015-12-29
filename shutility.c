#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <fcntl.h>

void display_info()
{
	char hostname[100] = {'\0'};
	struct passwd *p;
	
	p = getpwuid(getuid());
	gethostname(hostname, 99);
	printf("[%s@%s]# ",p->pw_name,hostname);
}

void redirection_input(char *word1, char *word2)
{

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