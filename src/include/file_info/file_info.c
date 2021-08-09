#define _GNU_SOURCE

#include "file_info.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

static inline int is_regular_file(mode_t mode)
{
    mode &= S_IFMT;
    if (mode != S_IFREG) return 0;
    return -1;
}

static void file_checksum(struct file_info *_file_info, unsigned char *_content)
{
    char *data;
    asprintf(&data, "%s%ld%s", _file_info->file_name,  _file_info->file_size, _content);
    SHA256(data, SHA256_DIGEST_LENGTH, _file_info->hash);
    for (int i=0; i < 32; i++) {
        sprintf(_file_info->checksum + (i*2), "%02x", _file_info->hash[i]);
    }

    free(data);
}

int file_info_init(struct file_info *_file_info)
{
    if (!_file_info) return -1;

    *_file_info = (struct file_info) {
        .file_name = NULL,
        .file_size = 0,
    };

    memset(_file_info->hash, 0, SHA256_DIGEST_LENGTH);
    memset(_file_info->checksum, 0, SHA256_DIGEST_LENGTH);

    return 0;
}

int file_info_load(const char *__restrict__ _filename, struct file_info *_file_info)
{
    if (!_filename || !_file_info) {
        fprintf(stderr, "Can't use null values as parameters!\n");
        return -1;
    }

    struct stat st;
    int fd;
    char *type;
    unsigned char content[512];

    if ((fd = open(_filename, O_RDONLY)) < 0) {
        fprintf(stderr, "Could not read the file %s\n", _filename);
        return -1;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(stderr, "Could not load %s stats\n", _filename);
        return -1;
    }

    if (read(fd, content, 1024) < 0) {
        fprintf(stderr, "Could not read %s contents\n", _filename);
        return -1;
    }

    if (!is_regular_file(st.st_mode)) {
        fprintf(stderr, "This file isn't a regular file!\n");
        close(fd);
        return -1;
    }

    _file_info->file_name = _filename,
    _file_info->file_size = st.st_size,

    file_checksum(_file_info, content);

    close(fd);
}