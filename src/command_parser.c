#include "shell.h"

int parseString(char * string, char ** parsed, char *sep)
{
	int index = 0;
	//char *sep = " \t\n\v\f\r";
	char *split = strtok(string,sep);
	//printf("split value %s",split);
	const char SINGLE_QUOTE[] = "\'";
	const char DOUBLE_QUOTE[] = "\"";
	while(split && index < MAX_COMMANDS)	
	{
		if(split[0]==SINGLE_QUOTE[0])
		{
			split[strlen(split)]= ' ';
			split = strtok(split,SINGLE_QUOTE);
		}
		else if(split[0]==DOUBLE_QUOTE[0])
		{
			split[strlen(split)]= ' ';
			split = strtok(split,DOUBLE_QUOTE);
		}		
		parsed[index++] = split;
		split = strtok(NULL, sep);
		//printf("split value %s",split);
	}
	parsed[index++] = NULL;	
	return 0;
	//printParsed(parsed); 
}
int count(char **multipleCommands)
{
	int count = 0;
	while(multipleCommands[count])
		++count;
	return count;
}


// FOR TESTING PURPOSES ONLY 
void printParsed(char **parsed)
{
	int i = 0;
	printf("The parsed command is: ");	
	while(parsed[i] != NULL)
	{
		printf(" %d: %s\n",i,parsed[i]);
		i++;	
	}
}

int errfunc(const char* epath, int errno)
{
	printf("Error while globbing. %s %d",epath,errno);
}
void doGlob(char **parsed)
{
	int i = 0;
	int flag = 0;
	glob_t pglob;
	char *temp[MAX_COMMANDS];
	while(parsed[i] != NULL)
	{
		temp[i] = strdup(parsed[i]);
		i++;
	}
	temp[i] = NULL;
	i = 0;
	while(parsed[i] != NULL)
	{
		if(strstr(parsed[i],"*") != NULL || strstr(parsed[i],"?") != NULL)
		{
			glob(parsed[i],GLOB_NOCHECK,errfunc,&pglob);	
			flag = 1;
			break; 
		}
		i++;
	}
	if(flag == 0)
		return;
	int j = 0;
	while(j < pglob.gl_pathc)
	{
		parsed[i + j] = strdup(pglob.gl_pathv[j]);
		j++;
	}
	i++;
	while(temp[i] != NULL)
	{
		parsed[i + j] = strdup(temp[i]);
		i++;
	}
	parsed[i+j -1] = NULL;
	//printParsed(parsed);
}
