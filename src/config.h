#ifndef CONFIG_H
#define CONFIG_H

typedef struct
{
    char exe[256];
    char args[256];
    char exclude[256][64];
    int exclude_count;
    char target[64];
    char command_format[512];
} Config;

void create_default_ini_file(const char *filename);
char *read_autobatch_file(const char *filename);
void trim_whitespace(char *str);
void parse_exclude_list(char *exclude_str, Config *config);
void parse_ini_file(const char *filename, Config *config);

#endif // CONFIG_H
