#include "shell.h"

/**
 * mainShellLoop - main shell loop
 * @info: the parameter and return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int mainShellLoop(info_t *info, char **av)
{
	ssize_t inputResult = 0;
	int builtinResult = 0;

	while (inputResult != -1 && builtinResult != -2)
	{
		clearInfo(info);
		if (isInteractive(info))
			printPrompt("$ ");
		flushErrorBuffer();
		inputResult = getInput(info);
		if (inputResult != -1)
		{
			setInfo(info, av);
			builtinResult = findBuiltin(info);
			if (builtinResult == -1)
				findCommand(info);
		}
		else if (isInteractive(info))
			printCharacter('\n');
		freeInfo(info, 0);
	}
	writeHistory(info);
	freeInfo(info, 1);
	if (!isInteractive(info) && info->status)
		exit(info->status);
	if (builtinResult == -2)
	{
		if (info->errorNumber == -1)
			exit(info->status);
		exit(info->errorNumber);
	}
	return (builtinResult);
}

/**
 * findBuiltin - finds a builtin command
 * @info: the parameter and return info struct
 *
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         2 if builtin signals exit()
 */
int findBuiltin(info_t *info)
{
	int i, builtinResult = -1;
	builtin_table builtintbl[] = {
		{"exit", myExit},
		{"env", myEnv},
		{"help", myHelp},
		{"history", myHistory},
		{"setenv", mySetenv},
		{"unsetenv", myUnsetenv},
		{"cd", myCd},
		{"alias", myAlias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->lineCount++;
			builtinResult = builtintbl[i].func(info);
			break;
		}
	return (builtinResult);
}

/**
 * findCommand - finds a command in PATH
 * @info: the parameter and return info struct
 *
 * Return: void
 */
void findCommand(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->lineCountFlag == 1)
	{
		info->lineCount++;
		info->lineCountFlag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!isDelimiter(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = findPath(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		forkCommand(info);
	}
	else
	{
		if ((isInteractive(info) || _getenv(info, "PATH=")
					|| info->argv[0][0] == '/') && isCommand(info, info->argv[0]))
			forkCommand(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			printError(info, "not found\n");
		}
	}
}

/**
 * forkCommand - forks an exec thread to run cmd
 * @info: the parameter and return info struct
 *
 * Return: void
 */
void forkCommand(info_t *info)
{
	pid_t childPid;

	childPid = fork();
	if (childPid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (childPid == 0)
	{
		if (execve(info->path, info->argv, getEnvironment(info)) == -1)
		{
			freeInfo(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				printError(info, "Permission denied\n");
		}
	}
}
