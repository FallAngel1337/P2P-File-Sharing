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

int jsonWriteFile(const char *_json, struct file_info *_file_info, char **_file_name)
{
    return 0;
}

char* jsonReadFile(const char *_file_name)
{
    return NULL;
}