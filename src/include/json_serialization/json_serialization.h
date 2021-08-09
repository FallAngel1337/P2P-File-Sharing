/**
 * json_serialization.h - Serialize a POD into a JSON format
 * Author: FallAngel
 * Date: 2021-08-09
 * 
 * Copyright (c) 2021
 */
/**
 * Serialize the `file_info` structure (see file_info.h for more information)
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

#include <cjson/cJSON.h>
#include "../file_info/file_info.h"

char *jsonSerialize(struct file_info *_file_info);
struct file_info* jsonDeserialize(const char *json);

#endif /* _JSON_SERIALIZATION */
