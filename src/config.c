#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

void create_default_ini_file(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error: Cannot create file %s\n", filename);
        return;
    }

    fprintf(file, "[config]\n");
    fprintf(file, "exe=;\n");
    fprintf(file, "args=;\n");
    fprintf(file, "exclude=;\n");
    fprintf(file, "target=;\n");
    fprintf(file, "command_format=;\n");
    printf("File %s created with default values.\n", filename);
    fclose(file);
}

char *read_autobatch_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        file = fopen(filename, "w");
        if (file == NULL)
        {
            printf("Error: Cannot create file %s\n", filename);
            return NULL;
        }

        fprintf(file, "default.ini");
        fclose(file);

        file = fopen(filename, "r");
        if (file == NULL)
        {
            printf("Error: Cannot open %s\n", filename);
            return NULL;
        }
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    char *content = (char *)malloc(filesize + 1);
    if (content == NULL)
    {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    fread(content, 1, filesize, file);
    content[filesize] = '\0';
    fclose(file);

    return content;
}

void trim_whitespace(char *str)
{
    char *end;
    while (*str == ' ')
        str++;
    if (*str == 0)
        return;
    end = str + strlen(str) - 1;
    while (end > str && *end == ' ')
        end--;
    end[1] = '\0';
}

void parse_exclude_list(char *exclude_str, Config *config)
{
    char *token = strtok(exclude_str, ",");
    config->exclude_count = 0;

    while (token != NULL)
    {
        trim_whitespace(token);
        strncpy(config->exclude[config->exclude_count], token, sizeof(config->exclude[0]) - 1);
        config->exclude_count++;
        token = strtok(NULL, ",");
    }
}

void parse_ini_file(const char *filename, Config *config)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        create_default_ini_file(filename);
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file))
    {
        trim_whitespace(line);
        if (line[0] == '\0' || line[0] == '[')
            continue;

        char *key = strtok(line, "=");
        char *value = strtok(NULL, "=");
        if (key && value)
        {
            trim_whitespace(key);
            trim_whitespace(value);
            value[strcspn(value, ";")] = 0;

            if (strcmp(key, "exe") == 0)
            {
                strncpy(config->exe, value, sizeof(config->exe) - 1);
            }
            else if (strcmp(key, "args") == 0)
            {
                strncpy(config->args, value, sizeof(config->args) - 1);
            }
            else if (strcmp(key, "exclude") == 0)
            {
                parse_exclude_list(value, config);
            }
            else if (strcmp(key, "target") == 0)
            {
                strncpy(config->target, value, sizeof(config->target) - 1);
            }
            else if (strcmp(key, "command_format") == 0)
            {
                strncpy(config->command_format, value, sizeof(config->command_format) - 1);
            }
        }
    }

    fclose(file);
}
