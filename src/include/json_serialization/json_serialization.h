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
 * `jsonWriteFile` write into `_file_name` a malloc'd string the name of the original file
 * described onto  `_file_info`. If `_file_name` is NULL it will simple not write
 * the string into it.
*/
int jsonWriteFile(char **_file_name, struct file_info *_file_info);
char* jsonReadFile(const char *_file_name, struct file_info *_file_info)

#endif /* _JSON_SERIALIZATION */
