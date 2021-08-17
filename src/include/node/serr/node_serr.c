#include "node_serr.h"
#include "../file_info/file_info.h"

#include <cjson/cJSON.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

/**
 * 
{
    "name": "Torrent File Share Network",
    "version": "0.1",
    "node_data": [
        {
            "file_name": "blah.txt",
            "file_size": 123,
            "checksum": "blahblahblah"
        },
        {
            "IP": "0.0.0.0",
            "PORT": 7622
        }
    ]
}
 */

char* nodeSerialize(struct Node *_node)
{
    return NULL;
}

struct Node* nodeDeserialize(const char *json, struct Node *_node)
{
    return NULL;
}

static char* change_file_extension(const char *_file_name, const char *__restrict__ _new_ext)
{
    char *__file_name = strdup(_file_name);
    strtok(__file_name, ".");
    strcat(__file_name, _new_ext);

    return __file_name;
}

int jsonWriteFile(char **_file_name, struct Node *_node)
{
    int fd, ret = 0;
    char *_new_filename;
    const char *_json = fileSerialize(_file_info);

    _new_filename = change_file_extension(_file_info->file_name, ".torrent");
    if (_file_name) *_file_name = _new_filename;

    if ((fd = open(_new_filename, O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR|S_IROTH)) < 0)
    {
        if (errno != EEXIST) {
            fprintf(stderr, "Could not open/create the %s file (%s)\n", _new_filename, strerror(errno));
            free(_new_filename);
            *_file_name = NULL;
        }
        close(fd);
        return -1;
    }

    if (write(fd, _json, strlen(_json)) < 0)
    {
        fprintf(stderr, "Could not write to the %s file (%s)\n", _new_filename, strerror(errno));
        free(_new_filename);
        *_file_name = NULL;
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

char* jsonReadFile(const char *_file_name, struct file_info *_node)
{
    struct stat st;
    int fd;
    char *buf;

    if ((fd = open(_file_name, O_RDONLY)) < 0)
    {   
        fprintf(stderr, "Could not open the %s file (%s)\n", _file_name, strerror(errno));
        return NULL;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(stderr, "Could not load %s file stats (%s)\n", _file_name, strerror(errno));
        close(fd);
        return NULL;
    }

    if (!(buf = calloc(1, st.st_size)))
    {
        fprintf(stderr, "malloc error (%s)\n", strerror(errno));
        close(fd);
        return NULL;
    }

    if (read(fd, buf, st.st_size) < 0)
    {
        fprintf(stderr, "Could not read %s file (%s)\n", _file_name, strerror(errno));
        close(fd);
        return NULL;
    }

    if(!jsonDeserialize(buf, _file_info))
    {
        fprintf(stderr, "Was not possible to deserialize %s (%s)\n", _file_name, strerror(errno));
        close(fd);
        return NULL;
    }

    close(fd);
    return buf;
}