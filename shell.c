#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "parsing.h"
#include "shutility.h"

/*
void sig_handler(int signo)
{
    	if (signo == SIGUSR1)
		printf("Received SIGUSR1\n");
	else if (signo == SIGTERM)
		printf("Received SIGTERM\n");
	else if (signo == SIGINT)
		printf("Received SIGINT\n");
	else if (signo == SIGFPE)
		printf("Received SIGFPE\n");
	else if (signo == SIGABRT)
		printf("Received SIGABRT\n");
}
*/

int main(int argc, char **argv)
{   
	int status = 1;
	int read;
	size_t len;
	char *buffer = NULL;

	/* Example of signal handling 
	* 
	*if (signal(SIGUSR1, sig_handler) == SIG_ERR)
	*	printf("\ncan't catch SIGUSR1\n");
	*if (signal(SIGTERM, sig_handler) == SIG_ERR)
	*	printf("\ncan't catch SIGTERM\n");
	*if (signal(SIGINT, sig_handler) == SIG_ERR)
	*	printf("\ncan't catch SIGINT\n");
	*if (signal(SIGFPE, sig_handler) == SIG_ERR)
	*	printf("\ncan't catch SIGFPE\n");	
	*if (signal(SIGABRT, sig_handler) == SIG_ERR)
	*	printf("\ncan't catch SIGABRT\n");
	*/
	
	printf("----- C O N S O L E\t S I M U L A T I O N -----\n\n");
	while (status) {    
		/* Get & display username and hostname */
		display_info();
		/* Get user input, strtok to remove newline */      
		read = getline(&buffer, &len, stdin);
		strtok(buffer, "\n");       
		status = parsing(buffer);
	}
	if (read)
		free(buffer);

	return 0;
}