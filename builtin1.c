#include "shell.h"

/**
 * display_history - Show the command history, starting at 0.
 * @info: Struct holding potential arguments.
 * Return: 0 after executing.
 */
int display_history(info_t *info)
{
    show_list(info->history);
    return (0);
}

/**
 * remove_alias - Remove an alias by name.
 * @info: Parameter structure.
 * @alias_name: Alias to remove.
 * Return: 0 if successful, 1 if an error occurred.
 */
int remove_alias(info_t *info, char *alias_name)
{
    char *separator, tmp;

    separator = find_char(alias_name, '=');
    if (!separator)
        return (1);
    tmp = *separator;
    *separator = '\0';
    int result = remove_node(&(info->alias), get_index(info->alias, starts_with(info->alias, alias_name)));
    *separator = tmp;

    return (result);
}

/**
 * define_alias - Assign a new alias.
 * @info: Parameter structure.
 * @alias_str: String in the format 'alias=value'.
 * Return: 0 if successful, 1 otherwise.
 */
int define_alias(info_t *info, char *alias_str)
{
    char *separator;

    separator = find_char(alias_str, '=');
    if (!separator)
        return (1);
    if (!*(separator + 1))
        return (remove_alias(info, alias_str));

    remove_alias(info, alias_str);
    return (append_to_list(&(info->alias), alias_str) == NULL);
}

/**
 * show_alias - Print the alias.
 * @item: Node containing the alias.
 * Return: 0 if successful, 1 if an error occurred.
 */
int show_alias(list_t *item)
{
    char *separator = NULL, *alias = NULL;

    if (item)
    {
        separator = find_char(item->str, '=');
        for (alias = item->str; alias <= separator; alias++)
            write_char(*alias);
        write_str("'");
        write_str(separator + 1);
        write_str("'\n");
        return (0);
    }
    return (1);
}

/**
 * handle_alias - Simulate the 'alias' command.
 * @info: Structure containing arguments.
 * Return: 0 upon completion.
 */
int handle_alias(info_t *info)
{
    int idx;
    char *separator = NULL;
    list_t *entry = NULL;

    if (info->argc == 1)
    {
        entry = info->alias;
        while (entry)
        {
            show_alias(entry);
            entry = entry->next;
        }
        return (0);
    }

    for (idx = 1; info->argv[idx]; idx++)
    {
        separator = find_char(info->argv[idx], '=');
        if (separator)
            define_alias(info, info->argv[idx]);
        else
            show_alias(matches_start(info->alias, info->argv[idx]));
    }

    return (0);
}
