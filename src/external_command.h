#ifndef EXTERNAL_COMMAND
#define EXTERNAL_COMMAND

#include<unistd.h>
#include<sys/types.h>

int externalCommand(char **parsed, int * fds);
int bgFork(char **parsed, int *fds);
int noWaitExec(char **parsed, int *fds);
int checkResult(pid_t pid1, pid_t pid2, int status);

#endif
