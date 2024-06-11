#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

void print_help()
{
    printf("Usage:\n");
    printf("  inspect -i <file_path> [-h] [-f format] [-l log_file] [-v]\n");
    printf("  inspect -a [directory_path] [-r] [-h] [-f format] [-l log_file] [-v]\n");
    printf("Options:\n");
    printf("  -i, --inode <file_path>    Display detailed inode information for the specified file.\n");
    printf("  -a, --all [directory_path] Display inode information for all files within the specified directory.\n");
    printf("  -r, --recursive            Recursively list all files in directories.\n");
    printf("  -h, --human                Output sizes in KB/MB/GB and dates in human-readable form.\n");
    printf("  -f, --format [text|json]   Specify output format (default: text).\n");
    printf("  -l, --log <log_file>       Log operations to a specified file.\n");
    printf("  -v, --verbose              Enable verbose output.\n");
    // printf("  -L, --log-level [error|warn|info|debug] Set the logging level.\n");
    printf("  -?, --help                 Display this help and exit.\n");
}

typedef enum
{
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG
} LogLevel;

static LogLevel current_log_level = LOG_WARN;

char *get_time_stamp()
{
    static char buffer[20];
    time_t now = time(NULL);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return buffer;
}

void log_message(FILE *log, LogLevel level, const char *message, ...)
{
    if (log && level <= current_log_level)
    {
        va_list args;
        va_start(args, message);
        fprintf(log, "[%s] ", get_time_stamp());
        vfprintf(log, message, args);
        fprintf(log, "\n");
        va_end(args);
    }
}

char *format_size(off_t size, int human)
{
    static char output[20];
    if (human)
    {
        if (size >= (1 << 30))
            sprintf(output, "%.2f GB", size / (double)(1 << 30));
        else if (size >= (1 << 20))
            sprintf(output, "%.2f MB", size / (double)(1 << 20));
        else if (size >= (1 << 10))
            sprintf(output, "%.2f KB", size / (double)(1 << 10));
        else
            sprintf(output, "%ld bytes", size);
    }
    else
    {
        sprintf(output, "%ld bytes", size);
    }
    return output;
}

char *format_time(time_t rawtime, int human)
{
    static char buffer[30];
    if (human)
    {
        struct tm *timeinfo = localtime(&rawtime);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    }
    else
    {
        sprintf(buffer, "%ld", rawtime);
    }
    return buffer;
}

void format_permissions(mode_t mode, char *buf)
{
    sprintf(buf, "%c%c%c%c%c%c%c%c%c",
            (mode & S_IRUSR) ? 'r' : '-',
            (mode & S_IWUSR) ? 'w' : '-',
            (mode & S_IXUSR) ? 'x' : '-',
            (mode & S_IRGRP) ? 'r' : '-',
            (mode & S_IWGRP) ? 'w' : '-',
            (mode & S_IXGRP) ? 'x' : '-',
            (mode & S_IROTH) ? 'r' : '-',
            (mode & S_IWOTH) ? 'w' : '-',
            (mode & S_IXOTH) ? 'x' : '-');
}

char *get_file_type(mode_t mode)
{
    if (S_ISREG(mode))
        return "regular file";
    if (S_ISDIR(mode))
        return "directory";
    if (S_ISLNK(mode))
        return "symbolic link";
    if (S_ISCHR(mode))
        return "character device";
    if (S_ISBLK(mode))
        return "block device";
    if (S_ISFIFO(mode))
        return "FIFO (named pipe)";
    if (S_ISSOCK(mode))
        return "socket";
    return "unknown";
}

int print_inode_info(char *filepath, int human, char *format, FILE *log, int verbose)
{
    struct stat fileInfo;
    if (stat(filepath, &fileInfo) != 0)
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: " ANSI_COLOR_RESET "Error getting file info for %s: %s\n", filepath, strerror(errno));
        if (log)
            fprintf(log, "Error getting file info for %s: %s\n", filepath, strerror(errno));
        return 1;
    }

    char permissions[10];
    format_permissions(fileInfo.st_mode, permissions);

    if (verbose)
    {
        printf("Processing file: %s\n", filepath);
        log_message(log, LOG_DEBUG, "Processing file: %s", filepath);
    }

    if (strcmp(format, "json") == 0)
    {
        // printf("{\n");
        // printf("  \"filePath\": \"%s\",\n", filepath);
        // printf("  \"inode\": {\n");
        // printf("    \"number\": %lu,\n", fileInfo.st_ino);
        // printf("    \"type\": \"%s\",\n", get_file_type(fileInfo.st_mode));
        // printf("    \"permissions\": \"%s\",\n", permissions);
        // printf("    \"linkCount\": %lu,\n", fileInfo.st_nlink);
        // printf("    \"uid\": %d,\n", fileInfo.st_uid);
        // printf("    \"gid\": %d,\n", fileInfo.st_gid);
        // printf("    \"size\": \"%s\",\n", format_size(fileInfo.st_size, human));
        // printf("    \"accessTime\": \"%s\",\n", format_time(fileInfo.st_atime, human));
        // printf("    \"modificationTime\": \"%s\",\n", format_time(fileInfo.st_mtime, human));
        // printf("    \"statusChangeTime\": \"%s\"\n", format_time(fileInfo.st_ctime, human));
        // printf("  }\n");
        // printf("}\n");
        printf(ANSI_COLOR_CYAN "{\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_YELLOW "  \"filePath\": " ANSI_COLOR_GREEN "\"%s\",\n" ANSI_COLOR_RESET, filepath);
        printf(ANSI_COLOR_CYAN "  \"inode\": {\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_YELLOW "    \"number\": " ANSI_COLOR_MAGENTA "%lu,\n" ANSI_COLOR_RESET, fileInfo.st_ino);
        printf(ANSI_COLOR_YELLOW "    \"type\": " ANSI_COLOR_GREEN "\"%s\",\n" ANSI_COLOR_RESET, get_file_type(fileInfo.st_mode));
        printf(ANSI_COLOR_YELLOW "    \"permissions\": " ANSI_COLOR_GREEN "\"%s\",\n" ANSI_COLOR_RESET, permissions);
        printf(ANSI_COLOR_YELLOW "    \"linkCount\": " ANSI_COLOR_MAGENTA "%lu,\n" ANSI_COLOR_RESET, fileInfo.st_nlink);
        printf(ANSI_COLOR_YELLOW "    \"uid\": " ANSI_COLOR_MAGENTA "%d,\n" ANSI_COLOR_RESET, fileInfo.st_uid);
        printf(ANSI_COLOR_YELLOW "    \"gid\": " ANSI_COLOR_MAGENTA "%d,\n" ANSI_COLOR_RESET, fileInfo.st_gid);
        printf(ANSI_COLOR_YELLOW "    \"size\": " ANSI_COLOR_GREEN "\"%s\",\n" ANSI_COLOR_RESET, format_size(fileInfo.st_size, human));
        printf(ANSI_COLOR_YELLOW "    \"accessTime\": " ANSI_COLOR_GREEN "\"%s\",\n" ANSI_COLOR_RESET, format_time(fileInfo.st_atime, human));
        printf(ANSI_COLOR_YELLOW "    \"modificationTime\": " ANSI_COLOR_GREEN "\"%s\",\n" ANSI_COLOR_RESET, format_time(fileInfo.st_mtime, human));
        printf(ANSI_COLOR_YELLOW "    \"statusChangeTime\": " ANSI_COLOR_GREEN "\"%s\"\n" ANSI_COLOR_RESET, format_time(fileInfo.st_ctime, human));
        printf(ANSI_COLOR_CYAN "  }\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN "}\n" ANSI_COLOR_RESET);
    }
    else
    {
        printf(ANSI_COLOR_CYAN "Information for %s:\n" ANSI_COLOR_RESET, filepath);
        printf("File Inode: " ANSI_COLOR_YELLOW "%lu\n" ANSI_COLOR_RESET, fileInfo.st_ino);
        printf("File Type: " ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, get_file_type(fileInfo.st_mode));
        printf("Permissions: " ANSI_COLOR_MAGENTA "%s\n" ANSI_COLOR_RESET, permissions);
        printf("Number of Hard Links: " ANSI_COLOR_BLUE "%lu\n" ANSI_COLOR_RESET, fileInfo.st_nlink);
        printf("Owner UID: " ANSI_COLOR_RED "%d\n" ANSI_COLOR_RESET, fileInfo.st_uid);
        printf("Group GID: " ANSI_COLOR_RED "%d\n" ANSI_COLOR_RESET, fileInfo.st_gid);
        printf("File Size: " ANSI_COLOR_YELLOW "%s\n" ANSI_COLOR_RESET, format_size(fileInfo.st_size, human));
        printf("Last Access Time: " ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, format_time(fileInfo.st_atime, human));
        printf("Last Modification Time: " ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, format_time(fileInfo.st_mtime, human));
        printf("Last Status Change Time: " ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, format_time(fileInfo.st_ctime, human));
    }

    if (log)
        fprintf(log, "Successfully retrieved information for %s\n", filepath);
    return 0;
}

int check_path_type(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
    {
        return -1;
    }
    if (S_ISREG(statbuf.st_mode))
    {
        return 0;
    }
    if (S_ISDIR(statbuf.st_mode))
    {
        return 1;
    }
    return -1;
}

void scan_directory(char *dirpath, int recursive, int human, char *format, FILE *log, int verbose)
{
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char path[1024];
    int first_entry = 1;

    if (!(dir = opendir(dirpath)))
    {
        fprintf(stderr, "Failed to open directory %s: %s\n", dirpath, strerror(errno));
        if (log)
        {
            fprintf(log, "Failed to open directory %s: %s\n", dirpath, strerror(errno));
        }
        return;
    }

    if (verbose)
    {
        printf("Scanning directory: %s\n", dirpath);
        log_message(log, LOG_INFO, "Scanning directory: %s", dirpath);
    }

    if (strcmp(format, "json") == 0)
    {
        printf("[\n");
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);
        if (stat(path, &statbuf) != 0)
        {
            fprintf(stderr, "Error getting stats for %s: %s\n", path, strerror(errno));
            if (log)
            {
                fprintf(log, "Error getting stats for %s: %s\n", path, strerror(errno));
            }
            continue;
        }

        if (!first_entry)
        {
            printf(",\n");
        }
        print_inode_info(path, human, format, log, verbose);
        first_entry = 0;

        if (recursive && S_ISDIR(statbuf.st_mode))
        {
            printf(",\n");
            scan_directory(path, recursive, human, format, log, verbose);
        }
    }

    if (strcmp(format, "json") == 0)
    {
        printf("\n]");
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    int opt;
    int human_readable = 0;
    char *format = "text";
    char *log_file = NULL;
    FILE *log = NULL;
    char *file_path = NULL;
    int inode_flag = 0, all_flag = 0, recursive_flag = 0;
    int verbose = 0;

    static struct option long_options[] = {
        {"help", no_argument, 0, '?'},
        {"inode", required_argument, 0, 'i'},
        {"all", optional_argument, 0, 'a'},
        {"recursive", no_argument, 0, 'r'},
        {"human", no_argument, 0, 'h'},
        {"format", required_argument, 0, 'f'},
        {"log", required_argument, 0, 'l'},
        {"verbose", no_argument, 0, 'v'},
        {0, 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, "?i:a::rhf:l:v", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'i':
            inode_flag = 1;
            file_path = optarg;
            break;
        case 'a':
            all_flag = 1;
            file_path = optarg ? optarg : ".";
            break;
        case 'r':
            recursive_flag = 1;
            break;
        case 'h':
            human_readable = 1;
            break;
        case 'f':
            format = optarg;
            break;
        case 'l':
            log_file = optarg;
            break;
        case 'v':
            verbose = 1;
            break;
        case 'L':
            if (strcmp(optarg, "debug") == 0)
                current_log_level = LOG_DEBUG;
            else if (strcmp(optarg, "info") == 0)
                current_log_level = LOG_INFO;
            else if (strcmp(optarg, "warn") == 0)
                current_log_level = LOG_WARN;
            else if (strcmp(optarg, "error") == 0)
                current_log_level = LOG_ERROR;
            break;
        case '?':
        default:
            print_help();
            return 0;
        }
    }

    if (!inode_flag && !all_flag)
    {
        fprintf(stderr, "No operation specified. Use -i or -a to specify an operation.\n");
        print_help();
        return 1;
    }

    if (log_file != NULL)
    {
        log = fopen(log_file, "a");
        if (!log)
        {
            // fprintf(stderr, "Error opening log file %s: %s\n", log_file, strerror(errno));
            fprintf(stderr, ANSI_COLOR_RED "Error: " ANSI_COLOR_RESET "Error opening log file %s: %s\n", log_file, strerror(errno));
            return 1;
        }
    }

    if (inode_flag || all_flag)
    {
        int path_type = check_path_type(file_path);
        if (path_type == -1)
        {
            fprintf(stderr, "Error: Path does not exist or is not accessible: %s\n", file_path);
            return 1;
        }
        if (inode_flag && path_type != 0)
        {
            fprintf(stderr, "Error: Path is not a file: %s\n", file_path);
            return 1;
        }
        if (all_flag && path_type != 1)
        {
            fprintf(stderr, "Error: Path is not a directory: %s\n", file_path);
            return 1;
        }

        if (inode_flag)
        {
            print_inode_info(file_path, human_readable, format, log, verbose);
        }
        if (all_flag)
        {
            scan_directory(file_path, recursive_flag, human_readable, format, log, verbose);
        }
    }

    if (log)
        fclose(log);
    return 0;
}
