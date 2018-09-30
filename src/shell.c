#include "shell.h"

//TODO: Expand capabilites to include no wait in process command.
// IN parseString, 2nd variable is output. Output needs to be created by calling function.
int current = 0;
int processCommand(char * string, char ** parsed)
{
	//NUll terminate string just in case.
	for(int i = 0; string[i] != 0; i++)
		if(string[i] == '\n')
			string[i] = 0;
	char *multipleCommands[MAX_COMMANDS];
	char *redirectedCommands[MAX_COMMANDS];
	parseString(string,multipleCommands,"|");
	int num = count(multipleCommands);
	int rcount;
	int appendFlag = 0;
	if(num == 1)
	{
		//Single command accpted. Check for cases: -	
		// 1. command < inp
		// 2. command > op or command >> op
		// 3. command < inp > op
		// 4. command > op < inp 
		
		int fds[] ={0,1};
		char * redirect2[MAX_COMMANDS];
		//parseString(multipleCommands[0],redirectedCommands, ">>");
		if(strstr(multipleCommands[0],">>") != NULL)
		{
			//Need to open file  in append mode.
			appendFlag = 1;
		}
		parseString(multipleCommands[0],redirectedCommands, ">");
		rcount = count(redirectedCommands);
		if(rcount > 1)
		{
			// Count is greater than one. Therfore >> or > exists.
			// Need to check if < existst in redirectedCommands[1] as we do not want it interfering.
			// below handles case 1 and 4.
			parseString(redirectedCommands[1],redirect2,"<");
			int n = count(redirect2);
			if(appendFlag == 1)
			{
				fds[1] = open(redirect2[0],O_WRONLY|O_APPEND);
			}
			else 
				fds[1] = creat(redirect2[0], 0644);
			if(n > 1)
			{ 
					fds[0] = open(redirect2[1],O_RDONLY);
			}			
			if(n == 1)
			{
				// check if < is there befor > case 3
			    parseString(redirectedCommands[0],redirect2,"<");
			    n = count(redirect2);
			    if(n > 1)
			    {
					fds[0] = open(redirect2[1],O_RDONLY);
				}
			}
			
		}
		// case 2
		parseString(multipleCommands[0],redirectedCommands,"<");
		rcount = count(redirectedCommands);
		if(rcount > 1)
		{
			fds[0] = open(redirectedCommands[1],O_RDONLY);
		}
		return doCommand(multipleCommands[0],parsed,fds);
		
	}
	int newPipe[] = {0,0};
	int fd[2];
	int i;	
	for( i = 0; i < num-1; i++)
	{
		fd[0] = newPipe[0];
		// need to parse 1st command for input redirection		
		if(i == 0)
		{
			parseString(multipleCommands[0],redirectedCommands, "<");
			rcount = count(redirectedCommands);
			if(rcount > 1)
				fd[0] = open(redirectedCommands[1],O_RDONLY);
		}
		pipe(newPipe);
		fd[1] = newPipe[1];
		doCommand(multipleCommands[i],parsed,fd);
		if(i > 0)
			close(fd[0]);
		close(fd[1]);
	}
	fd[0] = newPipe[0];
	fd[1] = 1;
	parseString(multipleCommands[i],redirectedCommands, ">");
	rcount = count(redirectedCommands);
	if(rcount > 1)
		fd[1] = creat(redirectedCommands[1],0644);
	doCommand(multipleCommands[i],parsed,fd);
	close(fd[0]);
	return 1;
}
int doCommand(char * string, char ** parsed,int *fds)
{
	// For now assumes that the commands given are not piped.
	parseString(string,parsed," \t\n\v\f\r");
	// Built in command executes built in comannd like cd and exit.	
	if(builtInCommand(parsed,fds))
		return 0;
	// externalCommand executes external command.	
	return externalCommand(parsed,fds);
	//return 1;
}
void init_shell(void)
{
	setenv("SHELL","/home/Desktop/git repos/ags-shell/shell",1);
}
void printHistory(char *history[])
{
	int i = current;
       	int hist_num = 1;
	do{
        	if (history[i])
		{
			printf("%4d  %s\n", hist_num, history[i]);
			hist_num++;
                }
		i = (i + 1) % MAX_HISTORY;

        }while (i != current);
}
int main(int argc, char *argv[])
{

	char inputCommands[MAX_INPUT_SIZE], *parsed[MAX_COMMANDS];
	char *history[MAX_HISTORY];
	for(int i =0; i < MAX_HISTORY; i++)
		history[i] = NULL;
	init_shell();
	char cwd[1000];
	while(1)
	{
		//getcwd(cwd,sizeof(cwd));
		//printf("Dir: %s$ ",cwd);
		if(getInput(inputCommands, history))
			continue;
		if(strcmp("history",inputCommands) == 0)
		{
			printHistory(history);
			continue;
		}
		if(strcmp("history\n",inputCommands) == 0)
		{
			printHistory(history);
			continue;
		}
		processCommand(inputCommands,parsed);
	}
	return 0;
}
int getInput(char *inputCommands,  char *history[])
{
	char *buf;
	char shell_prompt[MAX_PROMPT_SIZE];
	snprintf(shell_prompt, sizeof(shell_prompt), "%s:%s$ ", getenv("USER"), getcwd(NULL, 1024));
	buf = readline(shell_prompt);	
	char *multipleCommands[MAX_COMMANDS];
	char inpCopy[1000] = "";
	//char retCommand[MAX_COMMANDS][100];
	//fgets(buf,MAX_INPUT_SIZE,stdin);
	//printf("Read command: %s", buf);
	if(strlen(buf) != 0)
	{
		strcpy(inputCommands,buf);
		if(buf[strlen(buf) - 1] == '\\')
		{
			strcpy(inputCommands,"");
			while(buf[strlen(buf) - 1] == '\\')
			{
				buf[strlen(buf) - 1] = 0;
				strcat(inputCommands,buf);
				buf = readline("> ");
			}
			strcat(inputCommands,buf);
		}
		add_history(inputCommands);
		history[current] = strdup(inputCommands);
		current = (current + 1) % MAX_HISTORY;
		checkAlias(inputCommands,inputCommands);
		
		return 0;
	}
	else return 1;
}

