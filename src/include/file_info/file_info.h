/**
 * file_info.h - Describe a file
 * Author: FallAngel
 * Date: 2021-08-09
 * 
 * Copyright © 2021
*/
/**
 * Extract some file fields and
 * load them into a struct.
 * 
 * The struct will be used to verify
 * file integrity.
*/

#ifndef _FILE_INFO_H
#define _FILE_INFO_H

#include <sys/types.h>

// Need to be a REGULAR FILE
struct file_info {
    const char *file_name;
    size_t file_size; // in bytes

    char checksum[65]; // string representation of the sha256 hash
};

int file_info_init(struct file_info *_file_info);
int file_info_load(const char *__restrict__ _filename, struct file_info *_file_info);

#endif /* _FILE_INFO_H */