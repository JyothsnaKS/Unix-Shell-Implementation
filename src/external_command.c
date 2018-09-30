//TODO: Expand capabailites of external command to handles pipes and no wait.
#include "shell.h" 
int externalCommand(char **parsed, int *fds)
{

	int num = count(parsed);
	//printf("The Num is: %d\n",num);
	//printf("The value is: %c\n",parsed[num-1][0]);
	if(parsed[num-1][0] == '&')
	{
		// Need to execute as BG process
		//printf("HELLO!");
		parsed[num-1] = NULL;
		doGlob(parsed);
		return noWaitExec(parsed,fds);
	}
	doGlob(parsed);
	pid_t pid = fork();
	if(pid == -1)
	{
		printf("Failed forking.");
		return -1;
	}
	else if(pid == 0)
	{
		if(fds)
		{
			if(fds[0] != 0)
				dup2(fds[0],0);
			if(fds[1] != 1)
				dup2(fds[1],1);
		}
		if(execvp(parsed[0],parsed) < 0 )
		{
			printf("%s execution failed.\n",parsed[0]);
		}
		exit(0);	
	}
	else
	{
		waitpid(pid,NULL,WUNTRACED);
		return 1;
	}
}
int bgFork(char **parsed, int *fds)
{
	pid_t pid = fork();
	if(pid == 0)
	{
		if(fds)
		{
			if(fds[0] != 0)
				dup2(fds[0],0);
			if(fds[1] != 1)
				dup2(fds[1],1);
		}
		if(execvp(parsed[0],parsed) < 0)
		{
			printf("exec failed.\n");
		}
	}
	else if(pid > 0)
	{
		printf("Child is running in background.\n");
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}	
int noWaitExec(char **parsed, int *fds)
{
	int status;
	pid_t pid = fork();
	if(pid == 0)
	{
		bgFork(parsed,fds);
	}
	else if(pid > 0)
	{
		pid_t pid2 = pid;
		pid2 = waitpid(pid,&status,0);
		return checkResult(pid,pid2,status);
	}
	printf("Fork failed.\n");
	return -1;
}

int checkResult(pid_t pid1, pid_t pid2, int status)
{
	if(pid1 != pid2)
	{
		printf("Problem with fork!\n");
		return -1;
	}
	if(WIFEXITED(status))
	{
		return 1;
	}
	if(WIFSIGNALED(status))
	{
		printf("Child ended due to uncaught signal.\n");
		return -1;
	}
	if(WIFSTOPPED(status))
	{
		printf("Child Process has stopped.\n");
		return -1;
	}
	printf("Unknown error in child!\n");
	return -1;
}
