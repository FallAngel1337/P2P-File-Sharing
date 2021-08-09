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