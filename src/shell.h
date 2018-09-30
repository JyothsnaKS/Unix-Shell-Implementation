#ifndef SHELL
#define SHELL

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stddef.h> 
#include<readline/readline.h>
#include<readline/history.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<glob.h>

#include "built_in_command.h"
#include "command_parser.h"
#include "external_command.h"
#include "built_in_command.h"

#define MAX_COMMANDS 50
#define MAX_INPUT_SIZE 1000
#define MAX_PROMPT_SIZE 1000
#define MAX_HISTORY 10


typedef struct Alias
{
	char key[100];
	char value[100];
}Alias;

int processCommand(char * string, char ** parsed);
int getInput(char *inputCommands, char *history[]);
void printHistory(char *history[]);
void init_shell(void);
int doCommand(char * string, char ** parsed,int *fds);
#endif
