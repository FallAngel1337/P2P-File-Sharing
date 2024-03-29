#define _GNU_SOURCE

#include "file_info.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <limits.h>
#include <libgen.h>

static inline int is_regular_file(mode_t mode)
{
    mode &= S_IFMT;
    if (mode != S_IFREG) return 0;
    return -1;
}

static void file_checksum(struct file_info *_file_info, unsigned char *_content)
{
    char *data;
    unsigned char hash[32];
    asprintf(&data, "%s%ld%s", _file_info->file_name,  _file_info->file_size, _content);
    SHA256((unsigned char*)data, SHA256_DIGEST_LENGTH, hash);
    for (int i=0; i < 32; i++) {
        sprintf(_file_info->checksum + (i*2), "%02x", hash[i]);
    }

    free(data);
}

int file_info_init(struct file_info *_file_info)
{
    if (!_file_info) return -1;

    _file_info->file_size = 0,

    memset((char*)_file_info->file_name, 0, SHA256_DIGEST_LENGTH*2);
    memset(_file_info->checksum, 0, SHA256_DIGEST_LENGTH*2);

    return 0;
}

int file_info_load(const char *__restrict__ _filename, struct file_info *_file_info)
{
    if (!_filename || !_file_info || (strlen(_filename)+1) > FILENAME_MAX) {
        fprintf(stderr, "Invalid filename\n");
        return -1;
    }

    struct stat st;
    int fd;
    char *type;
    unsigned char *content;

    if ((fd = open(_filename, O_RDONLY)) < 0) {
        fprintf(stderr, "Could not read the file %s\n", _filename);
        return -1;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(stderr, "Could not load %s stats\n", _filename);
        return -1;
    }

    content = malloc(st.st_size);

    if (read(fd, content, st.st_size) < 0) {
        fprintf(stderr, "Could not read %s contents\n", _filename);
        return -1;
    }

    if (!is_regular_file(st.st_mode)) {
        fprintf(stderr, "This file isn't a regular file!\n");
        close(fd);
        return -1;
    }

    memcpy((char*)_file_info->file_name, basename((char*)_filename), MAX_FILENAME);
    _file_info->file_size = st.st_size,

    file_checksum(_file_info, content);
    
    free(content);
    close(fd);
    
    return  0;
}