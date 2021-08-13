/**
 * p2p-client.c - P2P Client
 * Author: FallAngel
 * Date: 2021-08-10
 * Version: 0.1
 * 
 * Copyright (c) 2021
 */
/**
 * Will interact with the server,
 * create and download the "torrents".
 * Also serve the file for downloading.
*/

#include "../include/file_info/file_info.h"
#include "../include/json_serialization/json_serialization.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static char* get_file_ext(const char *__restrict__ _filename)
{
    char *__filename = strdup(_filename);
    strtok(__filename, ".");
    __filename = strtok(NULL, ".");
    return __filename;
}

static inline bool isa_torrent(const char *_filename)
{
    char *_ext = get_file_ext(_filename);
    return (strcmp(_ext, "torrent")) ? false : true;
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        fprintf(stderr, "no arguments suplied\n");
        return -1;
    }

    struct file_info fileinfo;
    char *torrent_file;
    file_info_init(&fileinfo);

    if (isa_torrent(argv[1]))
    {
        /* Request server ... */
        jsonReadFile(argv[1], &fileinfo);
        printf("Filename: %s\n", fileinfo.file_name);
        printf("File size: %zu\n", fileinfo.file_size);
        printf("Checksum: %s\n", fileinfo.checksum);
    } else {
        file_info_load(argv[1], &fileinfo);
        jsonWriteFile(&torrent_file, &fileinfo);
        printf("Created %s\n", torrent_file);
        free(torrent_file);
    }
    return 0;
}