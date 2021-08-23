/**
 * node_serr.h - (De)Serialize a POD into a JSON format
 * Author: FallAngel
 * Date: 2021-08-09
 * 
 * Copyright (c) 2021
 */
/**
 * (De)Serialize the `node` structure (see node.h for more information)
 * into a JSON file format.
 * 
 * E.g:
 * {
 *      "name": "sample_file.txt",
 *      "size": 344,
 *      "checksum": "<sha256_hash_here>"
 * }
*/

#ifndef _NODE_SERR_H
#define _NODE_SERR_H

#include "../node.h"

// Special serialization flags
#define SERR_IP   1
#define SERR_PORT 2
#define SERR_NET  3

// Node serialization functions
char* nodeSerialize(struct Node *_node, uint8_t _flags);
struct Node* nodeDeserialize(const char *json, struct Node *_node, uint8_t _flags);


/**
 * `jsonWriteFile` load the `node` structure into a JSON file.
 * And write the new filename into `file_name` a malloc'd string.
*/
int jsonWriteFile(char **_file_name, struct Node *_node, uint8_t _flags);

/**
 * `jsonReadFile` read a JSON file indicated by `file_name`
 *  and fill the `node` structure. Return -1 on error.
 */
int jsonReadFile(const char *_file_name, struct Node *_node, uint8_t _flags);

#endif /* _NODE_SERR_H */
