#include "seeder.h"
#include <logging/logging.h>
#include <config/client/config.h>
#include <node/serr/node_serr.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BACKLOG 5

static char* find_file(struct file_info *fileinfo)
{
    DIR *d;
    struct dirent *dir;
    struct stat sb;

    if (!(d = opendir(CLIENT_TORRENTS))) {
        LOG_ERROR("Could not open %s directory :: %s\n", CLIENT_TORRENTS, strerror(errno));
        return NULL;
    }

    char *buf = calloc(1, PATH_MAX);
    chdir(CLIENT_TORRENTS);

    while ((dir = readdir(d))) 
    {
        if (strstr(dir->d_name, fileinfo->file_name)) {
            if (readlink(dir->d_name, buf, PATH_MAX) >= 0 
            && strstr(buf, fileinfo->file_name)             
            && stat(buf, &sb) >= 0                          
            && sb.st_size == fileinfo->file_size) 
            {
                break;    
            }
            
        } 
        dir = readdir(d);
    }

    rewinddir(d);
    closedir(d);

    return buf;
}

int sendfile(int _fd, struct Node *src)
{
    int err = 0;
    if (_fd <= 0 || !src) {
        err = -1; goto clean;
    }

    int sock = 0, fd = 0;

    char *file = NULL;
    char *data = calloc(1, src->fileinfo->file_size);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_ERROR("socket error: %s\n", strerror(errno));
        err = -1; goto clean;
    }

    file = find_file(src->fileinfo);
    LOG_INFO("file=%s\n", file);

    if ((fd = open(file, O_RDONLY)) < 0) {
        LOG_ERROR("Failed to open file %s :: %s\n", file, strerror(errno));
        err = -1; goto clean;
    }


    if (read(fd, data, src->fileinfo->file_size) < 0) {
        LOG_ERROR("Failed to read file %s :: %s\n", file, strerror(errno));
        err = -1; goto clean;
    }

    if (send(_fd, data, src->fileinfo->file_size, 0) < 0) {
        LOG_ERROR("Failed to send file %s :: %s\n", file, strerror(errno));
        err = -1; goto clean;
    }

clean:
    free(file);
    free(data);
    close(sock);
    close(fd);
    return err;
}

static int conn_handler(struct Node *_node)
{
    int sock = 0;
    int err = 0;
    struct sockaddr_in saddr = _node->addr;
    struct sockaddr_in caddr;
    socklen_t len = sizeof(caddr);
    memset(&caddr, 0, sizeof(caddr));

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_ERROR("Could not create socket :: %s", strerror(errno));
        err = -1; goto clean;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        LOG_ERROR("Failed to set socket options :: %s\n", strerror(errno));
        err = -1; goto clean;
    }

    if (bind(sock, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        LOG_ERROR("Failed to bind socket :: %s\n", strerror(errno));
        err = -1; goto clean;
    }

    if (listen(sock, BACKLOG) < 0) {
        LOG_ERROR("Failed to listen on socket :: %s\n", strerror(errno));
        err = -1; goto clean;
    }

    LOG_INFO("Listen at %s:%u\n", inet_ntoa(saddr.sin_addr), htons(saddr.sin_port));
    char *_buf = calloc(1, 512);

    while (1) {
        int connfd = accept(sock, (struct sockaddr*)&caddr, &len);
        LOG_INFO("Recive a file request from %s:%u\n", inet_ntoa(caddr.sin_addr), caddr.sin_port);
        
        if (recv(connfd, _buf, 512, 0) < 0) {
            LOG_ERROR("recv error: %s\n", strerror(errno));
            break;
        }

        struct Node *leecher = nodeDeserialize(_buf, NULL, 0);

        if (sendfile(connfd, leecher) == 0) {
            LOG_INFO("Send requested file ...\n");
        }

        close(connfd);
        memset(_buf, 0, 512);
    }

clean:
    free(_buf);
    close(sock);
    return err;
}

int seeder_start(struct Node *seeder)
{
    int fd;

    switch (fork()) {
        case -1:
            perror("fork()");
            return -1;
        case 0:
            break;
        default:
            _exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        perror("setsid()");
        return -1;
    }

    switch (fork()) {
        case -1:
            perror("fork()");
            return -1;
        case 0:
            break;
        default:
            _exit(EXIT_SUCCESS);
    }

    umask(0);
    if (chdir("/") < 0) {
       LOG_ERROR("Could not change to the root directory\n");
        return -1;
    }

    for (fd=0; fd < sysconf(_SC_OPEN_MAX); fd++) close(fd);

    fd = open("/dev/null", O_RDWR);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    
    LOG_INFO("Starting p2p daemon PID=%d ...\n", getpid());

    conn_handler(seeder);
    return 0;
}