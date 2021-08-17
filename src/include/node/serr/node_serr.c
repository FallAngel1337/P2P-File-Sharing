#include "node_serr.h"
#include "../../file_info/file_info.h"

#include <cjson/cJSON.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include <arpa/inet.h> // inet_ntoa

/**
 * 
{
    "version": "p2p-torrent-file-share 0.1",
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
    char *json = NULL;
    cJSON *version = NULL;
    cJSON *node_data = NULL;
    
    cJSON *file = NULL;
    cJSON *file_name = NULL;
    cJSON *file_size = NULL;
    cJSON *checksum = NULL;

    cJSON *addr = NULL;
    cJSON *ip = NULL;
    cJSON *port = NULL;

    cJSON *torrent = cJSON_CreateObject();
    if (!torrent) goto end;

    version = cJSON_CreateString("p2p-torrent-file-share 0.1");
    if (!version) goto end;

    cJSON_AddItemToObject(torrent, "version", version);

    node_data = cJSON_CreateArray();
    if (!node_data) goto end;

    cJSON_AddItemToObject(torrent, "node_data", node_data);

    // Adding file info to the array
    file = cJSON_CreateObject();
    if (!file) goto end;

    file_name = cJSON_CreateString(_node->fileinfo->file_name);
    file_size = cJSON_CreateNumber(_node->fileinfo->file_size);
    checksum = cJSON_CreateString(_node->fileinfo->checksum);

    cJSON_AddItemToObject(file, "file_name", file_name);
    cJSON_AddItemToObject(file, "file_size", file_size);
    cJSON_AddItemToObject(file, "checksum", checksum);

    cJSON_AddItemToArray(node_data, file);

    // Adding node address to the array
    addr = cJSON_CreateObject();
    if (!addr) goto end;

    ip = cJSON_CreateString(inet_ntoa(_node->addr.sin_addr));
    port = cJSON_CreateNumber(_node->addr.sin_port);

    cJSON_AddItemToObject(addr, "IP", addr);
    cJSON_AddItemToObject(addr, "PORT", port);

    cJSON_AddItemToArray(node_data, addr);

    json = cJSON_Print(torrent);
    if (!json) goto end;

end:
    cJSON_Delete(torrent);
    return json;
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
    const char *_json = nodeSerialize(_node);

    _new_filename = change_file_extension(_node->fileinfo->file_name, ".torrent");
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

char* jsonReadFile(const char *_file_name, struct Node *_node)
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

    if(!nodeDeserialize(buf, _node))
    {
        fprintf(stderr, "Was not possible to deserialize %s (%s)\n", _file_name, strerror(errno));
        close(fd);
        return NULL;
    }

    close(fd);
    return buf;
}