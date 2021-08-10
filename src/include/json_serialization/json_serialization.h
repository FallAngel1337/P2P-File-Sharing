/**
 * json_serialization.h - (De)Serialize a POD into a JSON format
 * Author: FallAngel
 * Date: 2021-08-09
 * 
 * Copyright (c) 2021
 */
/**
 * (De)Serialize the `file_info` structure (see file_info.h for more information)
 * into a JSON file format.
 * 
 * E.g:
 * {
 *      "name": "sample_file.txt",
 *      "size": 344,
 *      "checksum": "<sha256_hash_here>"
 * }
*/

#ifndef _JSON_SERIALIZATION
#define _JSON_SERIALIZATION

#include "../file_info/file_info.h"

char* jsonSerialize(struct file_info *_file_info);
struct file_info* jsonDeserialize(const char *json, struct file_info *_file_info);

/**
 * `jsonWriteFile` load the `_file_info` structure into a JSON file.
 * And write the new filename into `_file_name` a malloc'd string.
*/
int jsonWriteFile(char **_file_name, struct file_info *_file_info);

/**
 * `jsonReadFile` read a JSON file indicated by `_file_name`
 *  and fill the `_file_info` structure. Return NULL on error
 *  and a pointer to the JSON content.
 */
char* jsonReadFile(const char *_file_name, struct file_info *_file_info)

#endif /* _JSON_SERIALIZATION */
