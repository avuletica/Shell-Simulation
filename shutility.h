#ifndef SHUTILITIY_H
#define SHUTILITIY_H

void display_info();
void redirection_input(char *, char *);
void redirection_output(char *, char *, char *);
int execute_command(char *, char *);
void handle_pipe(char *, char *, char *, char *);

#endif