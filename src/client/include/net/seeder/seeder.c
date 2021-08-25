#include "seeder.h"
#include "../../../../include/logging/logging.h"
#include "../../../include/config/config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>

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

    if (!(dir = readdir(d))) {
        LOG_ERROR("Could not read %s directory entries :: %s\n", CLIENT_TORRENTS, strerror(errno));
        return NULL;
    }

    while ((dir = readdir(d))) 
    {
        printf(">> %s\n", dir->d_name);
    }

    rewinddir(d);
    closedir(d);

    return NULL;
}

int sendfile(struct Node *dst, struct Node *src)
{
    // if (!dst || !src) return -1;

    int sock = 0;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_ERROR("socket error: %s\n", strerror(errno));
        return -1;
    }
/*
    if (connect(sock, (struct sockaddr_in*)&dst->addr, sizeof(dst->addr)) < 0) {
        LOG_ERROR("Failed to connect to dst node: %s\n", strerror(errno));
        return -1;
    }
*/
    // if (send(sock, ) < 0)

    find_file(src->fileinfo);
    return 0;
}