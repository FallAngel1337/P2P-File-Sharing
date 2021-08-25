#include "seeder.h"
#include "../../../../include/logging/logging.h"
#include "../../../include/config/config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

static char* find_file(struct file_info *fileinfo)
{
    DIR *d;
    struct dirent *dir;

    if (!(d = opendir(CLIENT_TORRENTS))) {
        LOG_ERROR("Could not open %s directory :: %s\n", CLIENT_TORRENTS, strerror(errno));
        return NULL;
    }

    while ((dir = readdir(d))) 
    {
        if (strstr(dir->d_name, fileinfo->file_name)) break;
        dir = readdir(d);
    }

    rewinddir(d);
    closedir(d);

    char *buf = calloc(1, PATH_MAX);
    chdir(CLIENT_TORRENTS);
    readlink(dir->d_name, buf, PATH_MAX);
    return buf;
}

int sendfile(struct Node *dst, struct Node *src)
{
    if (!dst || !src) return -1;

    int sock = 0, fd = 0;
    int err = 0;

    char *file = NULL;
    char *data = calloc(1, src->fileinfo->file_size);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_ERROR("socket error: %s\n", strerror(errno));
        err = -1; goto clean;
    }

    file = find_file(src->fileinfo);

    if ((fd = open(file, O_RDONLY)) < 0) {
        LOG_ERROR("Failed to open file %s :: %s\n", file, strerror(errno));
        err = -1; goto clean;
    }

    printf("file=%s\n", file);

    if (read(fd, data, src->fileinfo->file_size) < 0) {
        LOG_ERROR("Failed to read file %s :: %s\n", file, strerror(errno));
        err = -1; goto clean;
    }

/*
    if (connect(sock, (struct sockaddr*)&dst->addr, sizeof(dst->addr)) < 0) {
        LOG_ERROR("Failed to connect to dst node: %s\n", strerror(errno));
        err = -1; goto clean;
    }

    if (send(sock, data, src->fileinfo->file_size, 0) < 0) {
        LOG_ERROR("Failed to send the data :: %s\n", strerror(errno));
    }
*/

clean:
    free(file);
    free(data);
    close(sock);
    close(fd);
    return err;
}