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

#include "../node.h"

// Node serialization functions
char* nodeSerialize(struct Node *_node);
struct Node* nodeDeserialize(const char *json, struct Node *_node);


/**
 * `jsonWriteFile` load the `_node` structure into a JSON file.
 * And write the new filename into `_file_name` a malloc'd string.
*/
int jsonWriteFile(char **_file_name, struct Node *_node);

/**
 * `jsonReadFile` read a JSON file indicated by `_file_name`
 *  and fill the `_node` structure. Return NULL on error
 *  and a pointer to the JSON content.
 */
char* jsonReadFile(const char *_file_name, struct Node *__node);

#endif /* _JSON_SERIALIZATION */
