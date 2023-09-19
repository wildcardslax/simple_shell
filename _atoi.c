#include "shell.h"

/* Determine if shell is operating in interactive mode. */
int is_interactive_mode(info_t *info)
{
    return (isatty(STDIN_FILENO) && info->readfd <= 2) ? 1 : 0;
}

/* Check if a character 'c' is part of the delimiter string 'delim'. */
int is_character_delimiter(char c, const char *delim)
{
    while (*delim)
    {
        if (c == *delim)
            return 1;
        delim++;
    }
    return 0;
}

/* Determine if character is an alphabet (A-Z, a-z). */
int is_alphabet(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) ? 1 : 0;
}

/* Convert string to integer, considering negative numbers and returning 0 if no numbers found. */
int str_to_int(const char *str)
{
    int result = 0, sign = 1, i = 0;

    while (str[i] == ' ')  // Skip leading spaces
        i++;

    if (str[i] == '-')  // Handle negative numbers
    {
        sign = -1;
        i++;
    }

    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + str[i] - '0';
        i++;
    }

    return result * sign;
}
