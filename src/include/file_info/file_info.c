#include "file_info.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct file_info {
    char *file_name;
    char *file_type;
    size_t file_size;

    unsigned char *checksum;
};

int file_info_init(struct file_info *_file_info)
{
    return 0;
}

int file_info_load(const char *__restrict__ _filename, struct file_info *_file_info)
{
    return 0;
}