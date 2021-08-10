#include "json_serialization.h"

#include <cjson/cJSON.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

char *jsonSerialize(struct file_info *_file_info)
{
    cJSON *file = cJSON_CreateObject();
    if (!file) {
        return NULL;
    }

    cJSON *file_name = NULL;
    cJSON *file_size = NULL;
    cJSON *checksum = NULL;
    char *json = NULL;

    file_name = cJSON_CreateString(_file_info->file_name);
    file_size = cJSON_CreateNumber(_file_info->file_size);
    checksum = cJSON_CreateString(_file_info->checksum);

    cJSON_AddItemToObject(file, "file_name", file_name);
    cJSON_AddItemToObject(file, "file_size", file_size);
    cJSON_AddItemToObject(file, "checksum", checksum);
    
    json = cJSON_Print(file);
    cJSON_Delete(file);
    return json;
}

struct file_info* jsonDeserialize(const char *json, struct file_info *_file_info)
{
    cJSON *file = cJSON_Parse(json);
    if (!file) {
        return NULL;
    }

    cJSON *file_name = NULL;
    cJSON *file_size = NULL;
    cJSON *checksum = NULL;

    file_name = cJSON_GetObjectItemCaseSensitive(file, "file_name");
    if (cJSON_IsString(file_name) && (file_name->valuestring != NULL))
    {
        _file_info->file_name = strdup(file_name->valuestring);
    }

    file_size = cJSON_GetObjectItemCaseSensitive(file, "file_size");
    if (cJSON_IsNumber(file_size))
    {
        _file_info->file_size = file_size->valueint;
    }

    checksum = cJSON_GetObjectItemCaseSensitive(file, "checksum");
    if (cJSON_IsString(checksum) && (checksum->valuestring != NULL))
    {
        strncpy(_file_info->checksum, checksum->valuestring, 64);
    }

    cJSON_Delete(file);
    return _file_info;
}

static char* change_file_extension(const char *_file_name, const char *__restrict__ _new_ext)
{
    char *__file_name = strdup(_file_name);
    strtok(__file_name, ".");
    strcat(__file_name, _new_ext);

    return __file_name;
}

int jsonWriteFile(char **_file_name, struct file_info *_file_info)
{
    int fd, ret = 0;
    char *_new_filename;
    const char *_json = jsonSerialize(_file_info);

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

char* jsonReadFile(const char *_file_name, struct file_info *_file_info)
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

    if (!(buf = malloc(st.st_size)))
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

    return buf;
}