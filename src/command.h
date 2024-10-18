#ifndef COMMAND_H
#define COMMAND_H

#include "config.h"

void build_command(char *command, const char *exe_path, const char *args, const char *input, const char *output, const char *command_format);
int is_excluded(const char *filename, Config *config);
void process_directory(const char *dir_path, Config *config);
void process_paths(int argc, char **argv, Config *config);

#endif // COMMAND_H
