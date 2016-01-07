#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char **argv) 
{
	int fp;
	int n=0;
  	char c='0';

	printf("This is test of redirection (<) !\n");

	fp = open(argv[1], O_RDONLY, S_IRUSR | S_IWUSR);
	if (!fp)
		perror("failed to open file");
	else {
		while ((n=read(fp, &c, 1)) > 0)
    		write(STDOUT_FILENO, &c, 1);
	}

	if (fp)
		close(fp);

}