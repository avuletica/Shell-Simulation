#include <stdio.h>
#include <pwd.h>
#include <unistd.h>

void display_info()
{
	char hostname[100] = {'\0'};
	struct passwd *p;
	
	p = getpwuid(getuid());
	gethostname(hostname, 99);
	printf("[%s@%s]# ",p->pw_name,hostname);
}