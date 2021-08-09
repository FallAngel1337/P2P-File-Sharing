#include "json_serialization.h"

#include <cjson/cJSON.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    checksum = cJSON_CreateRaw(_file_info->checksum);

    cJSON_AddItemToObject(file, "file_name", file_name);
    cJSON_AddItemToObject(file, "file_size", file_size);
    cJSON_AddItemToObject(file, "file_name", file_name);
    
    json = cJSON_Print(file);
    cJSON_Delete(file);
    return json;
}

struct file_info* jsonDeserialize(const char *json, struct file_info *_file_info)
{
    return NULL;
}

int jsonWriteFile(const char *_json, struct file_info *_file_info, char **_file_name)
{
    return 0;
}

char* jsonReadFile(const char *_file_name)
{
    return NULL;
}