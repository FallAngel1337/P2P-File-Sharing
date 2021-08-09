/**
 * file_info.h - Describe a file
 * Author: FallAngel
 * Date: 09/08/2021
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

    unsigned char checksum[32]; // sha256
};

int file_info_init(struct file_info *_file_info);
int file_info_load(const char *__restrict__ _filename, struct file_info *_file_info);

#endif /* _FILE_INFO_H */