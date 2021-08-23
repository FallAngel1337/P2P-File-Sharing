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
#include "../include/node/node.h"
#include "../include/node/serr/node_serr.h"

// Client headers
#include "include/config/config.h"
#include "include/net/conn/conn.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <libconfig.h>
#include <stdbool.h>

#include <arpa/inet.h>

static char* get_file_ext(const char *__restrict__ _filename, void **__free_later)
{
    char *__filename = strdup(_filename);
    *__free_later = __filename;
    strtok(__filename, ".");
    __filename = strtok(NULL, ".");
    return __filename;
}

static inline bool is_a_torrent(const char *_filename)
{
    void *_free_later;
    char *_ext = get_file_ext(_filename, &_free_later);
    bool result = (!_ext || strcmp(_ext, "torrent")) ? false : true;
    free(_free_later); return result;
}

static inline uint16_t use_random_port()
{
    srandom(time(NULL));
    return random() % 65536;
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

    char *filename = argv[1];
    int err = 0;
    config_init(&conf);

    if (config_read_file(&conf, CLIENT_CONFIG_FILE) == CONFIG_FALSE) {
        fprintf(stderr, "Can't load the configuration file %s :: %s\n", CLIENT_CONFIG_FILE, config_error_text(&conf));
        err = -1; goto clean;
    }

    config_lookup_string(&conf, "CLIENT_IP", &CLIENT_IP);
    config_lookup_int(&conf, "CLIENT_PORT", &CLIENT_PORT);
    config_lookup_string(&conf, "CLIENT_LOG_DIR", &CLIENT_LOG_DIR);
    
    config_lookup_string(&conf, "CSERVER_IP", &CSERVER_IP);
    config_lookup_int(&conf, "CSERVER_PORT", &CSERVER_PORT);

    if (CLIENT_PORT == 0 || CLIENT_PORT > 65535) 
    {
        printf("Invalid PORT value! Using a radomly choosen one ...\n");
        config_setting_t *PORT;
        CLIENT_PORT = use_random_port();
        PORT = config_lookup(&conf, "CLIENT_PORT");
        config_setting_set_int(PORT, CLIENT_PORT);
        config_write_file(&conf, CLIENT_CONFIG_FILE);
        printf("Writed %u to config file: %s\n", CLIENT_PORT, CLIENT_CONFIG_FILE);
    }

    struct Node *node = node_create(CLIENT_IP, CLIENT_PORT), *seeder;
    char *json = NULL;

    if (is_a_torrent(filename)) {
        if (jsonReadFile(filename, node, 0) < 0) {
            fprintf(stderr, "Could not load the json file_info content of the file %s\n", filename);
            err = -1; goto clean;
        }

        if (!(json = nodeSerialize(node, SERR_NET))) {
            fprintf(stderr, "Could not load the json addr info content of the file %s\n", filename);
            err = -1; goto clean;
        }
    } else {
        if (file_info_load(filename, node->fileinfo) < 0) {
            fprintf(stderr, "Could not load the file %s\n", filename);
            err = -1; goto clean;
        }

        if (jsonWriteFile(&filename, node, 0) < 0) {
            fprintf(stderr, "Could not create the torrent\n");
            err = -1; goto clean;
        }

        printf("Created %s successfully!\n", filename);

        if (!(json = nodeSerialize(node, SERR_NET))) {
            fprintf(stderr, "Was not possible to deserialize the node!\n");
            free(filename);
            err = -1; goto clean;
        }

        free(filename);
    }


    int nodefd = connectton(CSERVER_IP, CSERVER_PORT, json, strlen(json)+1);
    char buf[512];
    if (recvfromn(nodefd, buf, 512)) {
        err = -1; goto clean;
    }
    
    if (!(seeder = nodeDeserialize(buf, NULL, SERR_NET))) {
        err = -1; goto clean;
    }

    printf("IP: %s\nPORT: %d\n", inet_ntoa(seeder->addr.sin_addr), ntohs(seeder->addr.sin_port));
    
clean:
    config_destroy(&conf);
    node_destroy(node);
    node_destroy(seeder);
    free(json);       
    return err;
}