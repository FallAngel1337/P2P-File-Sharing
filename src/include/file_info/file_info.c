#include "file_info.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <openssl/sha.h>

struct file_info {
    char *file_name;
    char *file_type;
    size_t file_size; // in bytes

    unsigned char *checksum; // sha256
};

int file_info_init(struct file_info *_file_info)
{
    if (!_file_info) return -1;

    *_file_info = (struct file_info) {
        .file_name = NULL,
        .file_type = NULL,
        .file_size = 0,
        .checksum = NULL,
    };

    return 0;
}

int file_info_load(const char *__restrict__ _filename, struct file_info *_file_info)
{
    return 0;
}