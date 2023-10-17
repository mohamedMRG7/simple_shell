#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include "main.h"
/**
 * getmulticommands - Execute a command in a new process
 * @splitedcommands: Array of command parameters
 * @commands: Array of environment variables
 * Return: Result value of the command execution
 */
int getmulticommands(char **splitedcommands, char *commands)
{
	const char *delemter = ";";
	char *token;
	int i = 0;

	token = strtok(commands, delemter);
	while (token != NULL)
	{
		splitedcommands[i] = token;
		token = strtok(NULL, delemter);
		i++;
	}

	splitedcommands[i] = NULL;
	return (i);
}
/**
 * getEnvironmentVariable - Retrieve the value of an environment variable
 * @name: Name of the environment variable to retrieve
 * @environ: Array of environment variables
 * Return: Value of the environment variable, or NULL if not found
 */
char *getEnvironmentVariable(const char *name, char **environ)
{
	size_t nameLength = strlen(name);
	char **env;

	for (env = environ; *env != NULL; env++)
	{
		if (strncmp(*env, name, nameLength) == 0 &&
		    (*env)[nameLength] == '=')
		{
			return (*env + nameLength + 1);
		}
	}

	return (NULL);
}
/**
 * getExecutiblePath - Find the full path of an executable command
 * @command: Name of the command to search for
 * @env: Array of environment variables
 * Return: Full path of the executable command, or NULL if not found
 */
char *getExecutiblePath(char *command, char **env)
{
	struct stat st;
	const char *delemter = ":";
	char *token;
	int i;
	char *path;
	char *cpypath;

	path = getEnvironmentVariable("PATH", env);
	if (stat(command, &st) == 0)
	{
		return (command);
	}
	cpypath = malloc(sizeof(char) * strlen(path));
	strcpy(cpypath, path);
	i = 0;
	token = strtok(cpypath, delemter);
	while (token != NULL)
	{
		char *temp;

		temp = malloc(strlen(token) * sizeof(char) +
			      strlen(command) * sizeof(char) + 1);
		strcpy(temp, token);
		strcat(temp, "/");
		strcat(temp, command);
		if (stat(temp, &st) == 0)
		{
			free(cpypath);
			return (temp);
		}
		free(temp);
		token = strtok(NULL, delemter);
		i++;
	}

	free(cpypath);
	return (NULL);
}
/**
 * removenewline - Find the full path of an executable command
 * @bytesRead: Name of the command to search for
 * @line: Array of environment variables
 * Return: Full path of the executable command, or NULL if not found
 */
void removenewline(ssize_t bytesRead, char *line)
{
	if (bytesRead > 1 && line[bytesRead - 1] == '\n')
	{
		line[bytesRead - 1] = '\0';
		bytesRead--;
	}
}
