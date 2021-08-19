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

char* nodeSerialize(struct Node *_node, uint8_t _flags)
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
    if (_flags&SERR_NET) {
        addr = cJSON_CreateObject();
        if (!addr) goto end;

        if (_flags&SERR_IP) {
            ip = cJSON_CreateString(inet_ntoa(_node->addr.sin_addr));
            cJSON_AddItemToObject(addr, "ip", ip);
        }

        if (_flags&SERR_PORT) {
            port = cJSON_CreateNumber(_node->addr.sin_port);
            cJSON_AddItemToObjet(addr, "port", port);
        }

        cJSON_AddItemToArray(node_data, addr);
    }

    json = cJSON_Print(torrent);
    if (!json) goto end;

end:
    cJSON_Delete(torrent);
    return json;
}

struct Node* nodeDeserialize(const char *json, struct Node *_node, uint8_t _flags)
{
    if (!_node) _node = node_create("0.0.0.0", 9999);
    cJSON *version = NULL;
    cJSON *node_data = NULL;
    cJSON *data = NULL;

    cJSON *torrent = cJSON_Parse(json);
    if (!torrent) 
    {
        fprintf(stderr, "Could nopt parse the torrent file\n");
        cJSON_Delete(torrent);
        return NULL;
    }

    version = cJSON_GetObjectItemCaseSensitive(torrent, "version");
    if (!cJSON_IsString(version) || !version->valuestring)
    {
        fprintf(stderr, "Invalid version\n");
        cJSON_Delete(torrent);
        return NULL;
    }

    node_data = cJSON_GetObjectItemCaseSensitive(torrent, "node_data");
    
    cJSON_ArrayForEach(data, node_data)
    {
        cJSON *file_name = cJSON_GetObjectItemCaseSensitive(data, "file_name");
        cJSON *file_size = cJSON_GetObjectItemCaseSensitive(data, "file_size");
        cJSON *checksum = cJSON_GetObjectItemCaseSensitive(data, "checksum");
        cJSON *ip = cJSON_GetObjectItemCaseSensitive(data, "ip");
        cJSON *port = cJSON_GetObjectItemCaseSensitive(data, "port");
        memset(&_node->addr, 0, sizeof(_node->addr));
        _node->addr.sin_family = AF_INET;

        if (cJSON_IsString(file_name) && file_name->valuestring)
        {
            _node->fileinfo->file_name = file_name->valuestring;
        }

        if (cJSON_IsNumber(file_size))
        {
            _node->fileinfo->file_size = file_size->valueint;
        }

        if (cJSON_IsString(checksum) && checksum->valuestring)
        {
            memcpy(_node->fileinfo->checksum, checksum->valuestring, 65);
        }

        if (cJSON_IsString(ip) && ip->valuestring)
        {
            _node->addr.sin_addr.s_addr = inet_addr(ip->valuestring);
        }

        if (cJSON_IsNumber(port))
        {
            _node->addr.sin_port = htons(port->valueint);
        }
    }

    return _node;
}

static char* change_file_extension(const char *_file_name, const char *__restrict__ _new_ext)
{
    char *__file_name = strdup(_file_name);
    strtok(__file_name, ".");
    strcat(__file_name, _new_ext);

    return __file_name;
}

int jsonWriteFile(char **_file_name, struct Node *_node, uint8_t _flags)
{
    int fd, ret = 0;
    char *_new_filename;
    const char *_json = nodeSerialize(_node, _flags);

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

char* jsonReadFile(const char *_file_name, struct Node *_node, uint8_t _flags)
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

    if(!nodeDeserialize(buf, _node, _flags))
    {
        fprintf(stderr, "Was not possible to deserialize %s (%s)\n", _file_name, strerror(errno));
        close(fd);
        return NULL;
    }

    close(fd);
    return buf;
}