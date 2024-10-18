#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "command.h"

void build_command(char *command, const char *exe_path, const char *args, const char *input, const char *output, const char *command_format)
{
    strcpy(command, exe_path);

    char command_format_copy[512];
    strncpy(command_format_copy, command_format, sizeof(command_format_copy) - 1);
    command_format_copy[sizeof(command_format_copy) - 1] = '\0';

    char *token = strtok(command_format_copy, ",");
    while (token != NULL)
    {
        if (strcmp(token, "args") == 0 && strlen(args) > 0)
        {
            strcat(command, " ");
            strcat(command, args);
        }
        else if (strcmp(token, "input") == 0)
        {
            strcat(command, " ");
            strcat(command, "\"");
            strcat(command, input);
            strcat(command, "\"");
        }
        else if (strcmp(token, "output") == 0 && strlen(output) > 0)
        {
            strcat(command, " ");
            strcat(command, "\"");
            strcat(command, output);
            strcat(command, "\"");
        }
        token = strtok(NULL, ",");
    }
}

int is_excluded(const char *filename, Config *config)
{
    const char *ext = strrchr(filename, '.');
    if (ext == NULL)
        return 0;
    ext++;

    for (int i = 0; i < config->exclude_count; i++)
    {
        if (strcmp(ext, config->exclude[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void process_file(const char *file_path, Config *config)
{
    if (!is_excluded(file_path, config))
    {
        const char *original_path = file_path;
        char output_file[1024];

        if (strlen(config->target) > 0)
        {
            const char *ext = strrchr(original_path, '.');
            if (ext)
            {
                snprintf(output_file, sizeof(output_file), "%.*s.%s", (int)(ext - original_path), original_path, config->target);
            }
            else
            {
                snprintf(output_file, sizeof(output_file), "%s.%s", original_path, config->target);
            }
        }

        char command[1024];
        build_command(command, config->exe, config->args, original_path, output_file, config->command_format);
        printf("%s\n", command);
        printf("%s\n", original_path);

        int result = system(command);
        if (result == -1)
        {
            printf("Error: Command execution failed\n");
        }
    }
}

void process_directory(const char *dir_path, Config *config)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(dir_path)))
        return;

    while ((entry = readdir(dir)) != NULL)
    {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat path_stat;
        stat(full_path, &path_stat);

        if (S_ISDIR(path_stat.st_mode))
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                process_directory(full_path, config);
            }
        }
        else
        {
            if (!is_excluded(entry->d_name, config))
            {
                process_file(full_path, config);
            }
        }
    }
    closedir(dir);
}

void process_paths(int argc, char **argv, Config *config)
{
    for (int i = 1; i < argc; i++)
    {
        struct stat path_stat;
        stat(argv[i], &path_stat);

        if (S_ISDIR(path_stat.st_mode))
        {
            process_directory(argv[i], config);
        }
        else
        {
            process_file(argv[i], config);
        }
    }
}
