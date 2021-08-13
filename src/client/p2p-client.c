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

// Global headers
#include "../include/file_info/file_info.h"
#include "../include/json_serialization/json_serialization.h"

// Client headers
#include "include/config/config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>
#include <stdbool.h>

static char* get_file_ext(const char *__restrict__ _filename)
{
    char *__filename = strdup(_filename);
    strtok(__filename, ".");
    __filename = strtok(NULL, ".");
    return __filename;
}

static inline bool is_a_torrent(const char *_filename)
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

    config_t conf;
    const char *CLIENT_IP;
    unsigned CLIENT_PORT;
    const char *CLIENT_LOG_DIR;

    const char *CSERVER_IP;
    unsigned CSERVER_PORT;

    const char *filename = argv[1];

    config_init(&conf);

    if (config_read_file(&conf, CLIENT_CONFIG_FILE) == CONFIG_FALSE) {
        fprintf(stderr, "Can't load the configuration file %s :: %s\n", CLIENT_CONFIG_FILE, config_error_text(&conf));
        config_destroy(&conf);
        return -1;
    }


    config_lookup_string(&conf, "CLIENT_IP", &CLIENT_IP);
    config_lookup_int(&conf, "CLIENT_PORT", &CLIENT_PORT);
    config_lookup_string(&conf, "CLIENT_LOG_DIR", &CLIENT_LOG_DIR);
    
    config_lookup_string(&conf, "CSERVER_IP", &CSERVER_IP);
    config_lookup_int(&conf, "CSERVER_PORT", &CSERVER_PORT);

    if (is_a_torrent(filename)) {
        printf("It's a torrent!");
    } else {
        printf("It's not a torrent!");
    }


    config_destroy(&conf);
    return 0;
}