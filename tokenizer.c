#include "shell.h"

/**
 * **splitStringByDelimiters - splits a string into words. Repeated delimiters are ignored.
 * @str: the input string
 * @delimiters: the delimiter string
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **splitStringByDelimiters(char *str, char *delimiters)
{
	int i, j, k, m, numWords = 0;
	char **words;

	if (str == NULL || str[0] == 0)
		return (NULL);
	if (!delimiters)
		delimiters = " ";
	for (i = 0; str[i] != '\0'; i++)
		if (!isDelimiter(str[i], delimiters) && (isDelimiter(str[i + 1], delimiters) || !str[i + 1]))
			numWords++;

	if (numWords == 0)
		return (NULL);
	words = malloc((1 + numWords) * sizeof(char *));
	if (!words)
		return (NULL);
	for (i = 0, j = 0; j < numWords; j++)
	{
		while (isDelimiter(str[i], delimiters))
			i++;
		k = 0;
		while (!isDelimiter(str[i + k], delimiters) && str[i + k])
			k++;
		words[j] = malloc((k + 1) * sizeof(char));
		if (!words[j])
		{
			for (k = 0; k < j; k++)
				free(words[k]);
			free(words);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			words[j][m] = str[i++];
		words[j][m] = 0;
	}
	words[j] = NULL;
	return (words);
}

/**
 * **splitStringByDelimiter - splits a string into words
 * @str: the input string
 * @delimiter: the delimiter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **splitStringByDelimiter(char *str, char delimiter)
{
	int i, j, k, m, numWords = 0;
	char **words;

	if (str == NULL || str[0] == 0)
		return (NULL);
	for (i = 0; str[i] != '\0'; i++)
		if ((str[i] != delimiter && str[i + 1] == delimiter) ||
				    (str[i] != delimiter && !str[i + 1]) || str[i + 1] == delimiter)
			numWords++;
	if (numWords == 0)
		return (NULL);
	words = malloc((1 + numWords) * sizeof(char *));
	if (!words)
		return (NULL);
	for (i = 0, j = 0; j < numWords; j++)
	{
		while (str[i] == delimiter && str[i] != delimiter)
			i++;
		k = 0;
		while (str[i + k] != delimiter && str[i + k] && str[i + k] != delimiter)
			k++;
		words[j] = malloc((k + 1) * sizeof(char));
		if (!words[j])
		{
			for (k = 0; k < j; k++)
				free(words[k]);
			free(words);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			words[j][m] = str[i++];
		words[j][m] = 0;
	}
	words[j] = NULL;
	return (words);
}
