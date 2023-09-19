#include "shell.h"

/**
 * shell_exit - Facilitate exit from the shell.
 * @info: Structure holding command arguments and other info.
 * Return: Exits with the specified exit status or
 *         returns (0) if info.argv[0] != "exit".
 */
int shell_exit(info_t *info)
{
    int exit_status;

    if (info->argv[1])  /* Checking for exit argument */
    {
        exit_status = parse_error_integer(info->argv[1]);
        if (exit_status == -1)
        {
            info->status = 2;
            display_error_message(info, "Invalid number: ");
            custom_print(info->argv[1]);
            custom_putchar('\n');
            return (1);
        }
        info->err_num = parse_error_integer(info->argv[1]);
        return (-2);
    }
    info->err_num = -1;
    return (-2);
}

/**
 * shell_change_dir - Alter the shell's working directory.
 * @info: Structure holding command arguments and other info.
 * Return: 0 on successful directory change.
 */
int shell_change_dir(info_t *info)
{
    char *current_dir, *target_dir, dir_buffer[1024];
    int dir_change_status;

    current_dir = getcwd(dir_buffer, 1024);
    if (!current_dir)
        custom_print("Error: Could not fetch current directory.\n");

    if (!info->argv[1])
    {
        target_dir = fetch_environment_var(info, "HOME=");
        dir_change_status = target_dir ? chdir(target_dir) : chdir(fetch_environment_var(info, "PWD="));
    }
    else if (str_compare(info->argv[1], "-") == 0)
    {
        if (!fetch_environment_var(info, "OLDPWD="))
        {
            custom_print(current_dir);
            custom_putchar('\n');
            return (1);
        }
        custom_print(fetch_environment_var(info, "OLDPWD="));
        custom_putchar('\n');
        dir_change_status = chdir(fetch_environment_var(info, "OLDPWD="));
    }
    else
    {
        dir_change_status = chdir(info->argv[1]);
    }

    if (dir_change_status == -1)
    {
        display_error_message(info, "Failed to change directory to ");
        custom_print(info->argv[1]);
        custom_putchar('\n');
    }
    else
    {
        update_environment_var(info, "OLDPWD", fetch_environment_var(info, "PWD="));
        update_environment_var(info, "PWD", getcwd(dir_buffer, 1024));
    }
    return (0);
}

/**
 * shell_help - Display help messages for shell commands.
 * @info: Structure holding command arguments and other info.
 * Return: 0 after printing help messages.
 */
int shell_help(info_t *info)
{
    (void)info; // Ignoring unused parameter

    custom_print("Help command invoked. Awaiting full implementation.\n");
    return (0);
}
