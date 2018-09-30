#include "shell.h"
int changeDir(char **parsed)
{
	if(parsed[1]==NULL) 
		parsed[1] = "/home/hiranmaya";
	if(strcmp(parsed[1]," ")==0)
		parsed[1] = "/home/hiranmaya";
	int retVal = chdir(parsed[1]);
	if(retVal == 0)
		return 1;
	else 
		printf("chdir failed.\n");
	return -1;
}

Alias alias[10];
int top = 0;
int addAlias(char **parsed)
{
	char *aliasCommand[MAX_COMMANDS];
	parseString(parsed[1],aliasCommand,"=");
	// we need to cat all the values after = as it might have spaces
	int i = 2;
	char value[100] = "";
	strcat(value,aliasCommand[1]);
	while(parsed[i] != NULL)
	{
		strcat(value," ");
		strcat(value,parsed[i]);
		i++;
	}
	// key is aliasCommand[0]
	// value is aliasCommand[1]
	if(top == 10)
	{
		printf("Max Alias limit reached.");
		return -1;
	}	
	strcpy(alias[top].key,aliasCommand[0]);
	strcpy(alias[top].value,value);
	//printf("Key check: %s\n",alias[top].key);
	//printf("Value check: %s\n",alias[top].value);
	top++;
	return 1;
	
}

int checkAlias(char* key, char *op)
{
	for(int i = 0; i<top;i++)
	{
		if(strcmp(alias[i].key,key) == 0)
		{
			strcpy(op,alias[i].value);
			return 1;
		}
	}
	strcpy(op,key);
	return 0;
}
int sgown(char **parsed)
{
	char *temp = strdup(parsed[1]);
	int fds[] = {0,1};
	parsed[0] = strdup("grep");
	parsed[1] = strdup("-rn");
	parsed[2] = strdup(temp);
	parsed[3] = strdup("*");
	parsed[4] = NULL;
	return externalCommand(parsed,fds);
}
int zps(char **parsed)
{
	char *string = strdup("ps -al | grep defunct");
	return processCommand(string,parsed);
}
int builtInCommand(char **parsed, int *fds)
{
	switch(checkInBuiltCommand(parsed[0]))
	{
		case 1:
			return changeDir(parsed);break;
		case 2:
			printf("\n\nThank you for using ags-shell made by A. Harshith, G. Hiranmaya, S. Jyothsna.\n\n\n");
			exit(0);break;
		case 3:
			return addAlias(parsed);
		case 4:
			return sgown(parsed);
		case 5:
			return zps(parsed);
		default: 
			return 0;
	}
}

int checkInBuiltCommand(char *command)
{
	// Checks to see if the command exists.
	//printf("The command is: %s\n",command);
	if(command == NULL)
		return 8;
	if(strcmp(command,"sgown") == 0)
		return 4;
	if(strcmp(command,"zps") == 0)
		return 5;
	if(strlen(command) == 0)
		return 6;
	if(strcmp(command,"cd") == 0)
		return 1;
	if(strcmp(command,"exit") == 0)
	{
		return 2;
	}
        if(strcmp(command,"history") == 0)
		return 7;
	if(strcmp(command,"alias") == 0)
		return 3;
	return 9;
}
