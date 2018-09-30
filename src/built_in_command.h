#ifndef BUILT_IN_COMMAND
#define BUILT_IN_COMMAND

#include<string.h>
int changeDir(char **parsed);
int builtInCommand(char **parsed, int * fds);
int checkInBuiltCommand(char *command);
int addAlias(char **parsed);
int checkAlias(char* key, char *op);

#endif
