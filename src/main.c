#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "config.h"
#include "command.h"

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");

    Config config = {0};
    char current_dir[PATH_MAX];

#ifdef _WIN32
    HMODULE hModule = GetModuleHandle(NULL);
    GetModuleFileName(hModule, current_dir, sizeof(current_dir));
    char *last_slash = strrchr(current_dir, '\\');
    if (last_slash)
        *last_slash = '\0';
#else
    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
    {
        perror("Error getting current directory");
        return 1;
    }
#endif

    char autobatch_path[PATH_MAX];
    snprintf(autobatch_path, sizeof(autobatch_path), "%s/autobatch.txt", current_dir);
    char *ini_filename = read_autobatch_file(autobatch_path);
    char ini_path[PATH_MAX];
    snprintf(ini_path, sizeof(ini_path), "%s/%s", current_dir, ini_filename);
    parse_ini_file(ini_path, &config);

    if (argc < 2)
    {
        printf("Usage: Drag and drop files or folders onto the executable\n");
        return 1;
    }

    process_paths(argc, argv, &config);

    printf("Press Enter to exit...\n");
    getchar();

    return 0;
}
