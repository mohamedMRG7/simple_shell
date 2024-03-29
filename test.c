#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include "main.h"
/**
 * setparams - Tokenize a command string into parameters
 * @params: Array to store command parameters
 * @command: Input command string
 */
void setparams(char **params, char *command)
{
	const char *delemter = " ";
	char *token;
	int i = 0;

	token = strtok(command, delemter);
	while (token != NULL)
	{
		params[i] = token;
		token = strtok(NULL, delemter);
		i++;
	}
	params[i] = NULL;
	if (strcmp(params[0], "exit") == 0)
	{
		exit(params[1] == NULL ? 0 : atoi(params[1]));
	}
}
/**
 * executecommand - Execute a command in a new process
 * @execPath: Full path of the command executable
 * @params: Array of command parameters
 * @env: Array of environment variables
 * @appname: Name of the application
 * Return: Result value of the command execution
 */
int executecommand(char *execPath, char **params, char **env, char *appname)
{
	int pid;
	int resultvals;

	if (execPath == NULL)
	{
		perror(appname);
		resultvals = -1;
		return (resultvals);
	}
	pid = fork();
	if (pid < 0)
	{
		resultvals = -1;
	}
	if (pid == 0)
	{
		if (execve(execPath, params, env) == -1)
		{
			perror(appname);
			resultvals = -1;
		}
		resultvals = 0;
	}
	else
	{
		wait(NULL);
	}
	return (resultvals);
}
/**
 * init - Execute a command in a new process
 * @ac: Array of command parameters
 * @bufferSize: Array of environment variables
 * @resultvals: Name of the application
 * Return: Result value of the command execution
 */
void init(int *ac, size_t *bufferSize, int *resultvals)
{
	(void) ac;
	(void) bufferSize;
	(void) resultvals;
	bufferSize = 0;
	resultvals = 0;
}
/**
 * main - Entry point of the program
 * @ac: Number of command-line arguments
 * @av: Array of command-line arguments
 * @env: Array of environment variables
 * Return: Exit status of the program
 */
int main(int ac, char **av, char **env)
{
	char *line, **params, *execPath, **scomands;
	size_t bufferSize;
	ssize_t bytesRead;
	int resultvals, s, i;

	init(&ac, &bufferSize, &resultvals);
	line = NULL;
	if (isatty(STDIN_FILENO))
		printf("($) ");
	while ((bytesRead = getline(&line, &bufferSize, stdin)) > -1)
	{
		removenewline(bytesRead, line);
		params = malloc((strlen(line) + 1) * sizeof(char *));
		scomands = malloc((strlen(line) + 1) * sizeof(char *));
		if (params == NULL || scomands == NULL)
		{
			resultvals = -1;
			break;
		}
		s = getmulticommands(scomands, line);
		for (i = 0; i < s; i++)
		{
			setparams(params, scomands[i]);
			execPath = getExecutiblePath(params[0], env);
			resultvals = executecommand(execPath, params, env, av[0]);
			
		}
		if (isatty(STDIN_FILENO))
			printf("($) ");
		free(params);
		free(scomands);
	}
	free(line);
	return (resultvals);
}
