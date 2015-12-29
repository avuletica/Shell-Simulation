#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsing.h"
#include "shutility.h"

int main(int argc, char **argv)
{	
	int status = 1;
	int read;
	size_t len;
	char *buffer = NULL;

	printf("----- C O N S O L E\t S I M U L A T I O N -----\n\n");
	while (status) {	
		/* Get & display username and hostname */
		display_info();
		/* Get user input, strtok to remove newline */		
		read = getline(&buffer, &len, stdin);
		strtok(buffer, "\n");		
		status = parsing(buffer);
	}
	if(read)
		free(buffer);

	return 0;
}
