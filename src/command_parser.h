#ifndef COMMAND_PARSER
#define COMMAND_PARSER

#include<string.h>

int parseString(char * string, char ** parsed, char *sep);
int count(char **);
void printParsed(char **parsed);
int errfunc(const char* epath, int errno);
void doGlob(char **parsed);
#endif
