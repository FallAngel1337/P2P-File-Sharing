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

#ifndef _GNU_SOURCE
#   define _GNU_SOURCE
#endif

// Global headers
#include <node/node.h>
#include <node/serr/node_serr.h>

// Client headers
#include <config/client/config.h>
#include <net/conn/conn.h>
#include <net/seeder/seeder.h>

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <libconfig.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <limits.h>

#include <arpa/inet.h>

static char* get_file_ext(const char *__restrict__ _filename, void **__free_later)
{
    char *__filename = strdup(_filename);
    *__free_later = __filename;
    strtok(__filename, ".");
    __filename = strtok(NULL, ".");
    return __filename;
}

static inline uint16_t use_random_port(void)
{
    srandom(time(NULL));
    return random() % 65536;
}

static inline bool is_a_torrent(const char *__restrict__ _filename)
{
    void *_free_later;
    char *_ext = get_file_ext(_filename, &_free_later);
    bool result = (!_ext || strcmp(_ext, "torrent")) ? false : true;
    free(_free_later); return result;
}


static inline bool is_absolute_path(const char *__restrict__ _path)
{
    return (*_path == '/') ? true : false;
}

static int create_torrent_dir(const char *__restrict__ _dir)
{
    struct stat sb;
    if (!stat(_dir, &sb) && S_ISDIR(sb.st_mode)) {
        // printf("Directory already exists!\n");
        return 1;
    } else {
        if (mkdir(_dir, 0777) < 0) {
            fprintf(stderr, "mkdir failed :: %s\n", strerror(errno));
            return -1;
        }
    }

    return 0;
}

static int create_symlink(const char *__restrict__ old_filename,
                          const char *__restrict__ new_filename)
{
    if (!old_filename || !new_filename) return -1;
    if (create_torrent_dir(CLIENT_TORRENTS) < 0) return -1;
    char *link;
    int fd = 0;

    if (asprintf(&link, "%s/%s_XXXXXX", CLIENT_TORRENTS, new_filename) < 0) {
        fprintf(stderr, "asprintf failed :: %s\n", strerror(errno));
        return -1;
    }

    if ((fd = mkstemp(link)) < 0) {
        fprintf(stderr, "mkstemp failed :: %s\n", strerror(errno));
        free(link);
        return -1;
    }

    remove(link);

    if (is_absolute_path(old_filename)) 
    {
        if (symlink(old_filename, link) < 0) {
            fprintf(stderr, "Something gone wrong :: %s\n", strerror(errno));
            free(link);
            close(fd);
            return -1;
        }
    } else {
        char *__old_filename = realpath(old_filename, NULL);

        if (symlink(__old_filename, link) < 0) {
            fprintf(stderr, "Something gone wrong :: %s\n", strerror(errno));
            free(link);
            free(__old_filename);
            close(fd);
            return -1;
        }
        free(__old_filename);
    }

    free(link);
    close(fd);
    return 0;
}

static int savefile(const char *__restrict__ _filename, const char *_content, size_t _n)
{
    char *savefileto;
    int fd = 0;
    asprintf(&savefileto, "%s/%s", CLIENT_DOWNLOAD, _filename);

    if ((fd = open(savefileto, O_WRONLY|O_CREAT|O_TRUNC|O_EXCL, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH|S_IROTH)) < 0) {
        fprintf(stderr, "Could not save the file to \"%s\" :: %s\n", _filename, strerror(errno));
        free(savefileto);
        return -1;
    }

    if (write(fd, _content, _n) < 0) {
        fprintf(stderr, "Could not write to \"%s\" :: %s\n", _filename, strerror(errno));
        free(savefileto);
        return -1;
    }

    printf("Saved the file at: %s\n", _filename);
    free(savefileto);
    return 0;
}

// Global configuration variables;
const char *CLIENT_IP;
unsigned CLIENT_PORT;
const char *CLIENT_LOG_DIR;
const char *CLIENT_TORRENTS;
const char *CLIENT_DOWNLOAD;

const char *CSERVER_IP;
unsigned CSERVER_PORT;

static struct option long_options[] = {
    {"clientIP", required_argument, NULL, 'a'},
    {"clientPort", required_argument, NULL, 'b'},
    {"clientLogDir", required_argument, NULL, 'c'},
    {"clientTorrentDir", required_argument, NULL, 'd'},
    {"clientDownload", required_argument, NULL, 'e'},
    {"default", no_argument, NULL, 'f'},
    {"help", no_argument, NULL, 'h'},
    {0, 0, 0, 0},
};

static void __attribute__((noreturn)) help(char *__restrict__ __progname) 
{
    printf("Usage: %s <file> [options] ...\n", __progname);
    printf("\nOPTIONS:\n");

    printf("\t--clientIP\t\tSet the client IP on which other will connect to\n");
    printf("\t--clientPort\t\tSet the client port on which other will connect to\n");
    printf("\t--clientLogDir\t\tDefine on where the logs will be saved (recommend the defult)\n");
    printf("\t--clientTorrentDir\tThe path where the links will be created (recommend the defult)\n");
    printf("\t--clientDownload\tThe path where the downloads will be saved\n");
    printf("\t--default\t read from the config file\n");

    printf("\nExample:\n");
    printf("\t%s loveletter.txt\n", __progname);
    printf("\t%s /home/john/loveletter.txt\n", __progname);
    printf("\t%s /home/john/loveletter.txt --clientIP \"127.0.0.1\"\n", __progname);

    exit(EXIT_FAILURE);
}


int main(int argc, char **argv)
{
    if (argc == 1) {
        fprintf(stderr, "Too few arguments!\n");
        help(argv[0]);
    }
    config_t conf;

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

    config_lookup_string(&conf, "CLIENT_TORRENTS", &CLIENT_TORRENTS);
    config_lookup_string(&conf, "CLIENT_DOWNLOAD", &CLIENT_DOWNLOAD);

    config_lookup_string(&conf, "CSERVER_IP", &CSERVER_IP);
    config_lookup_int(&conf, "CSERVER_PORT", &CSERVER_PORT);

    int c = 1;
    int index = 0;

    while (c) 
    {
        c = getopt_long(argc, argv, "a:b:c:d:e:h", long_options, &index);

        if (c == -1) break;

        switch (c) {
           case 0:
                break;
            case 'a':
                CLIENT_IP = strdup((const char*)optarg);
                if (!strcmp(CLIENT_IP, "")) break;
                config_setting_t *IP = config_lookup(&conf, "CLIENT_IP");
                config_setting_set_string(IP, CLIENT_IP);
                config_write_file(&conf, CLIENT_CONFIG_FILE);
                break;

            case 'b':
                CLIENT_PORT = atoi(optarg);
                if (CLIENT_PORT == 0 || CLIENT_PORT > 65535) 
                {
                    CLIENT_PORT = use_random_port();
                    printf("Invalid PORT value! Using a radomly choosen one ...\n");
                }
                config_setting_t *PORT = config_lookup(&conf, "CLIENT_PORT");
                config_setting_set_int(PORT, CLIENT_PORT);
                config_write_file(&conf, CLIENT_CONFIG_FILE);
                break;

            case 'c':
                CLIENT_LOG_DIR = strdup((const char*)optarg);
                if (!strcmp(CLIENT_LOG_DIR, "")) break;
                config_setting_t *LOGS = config_lookup(&conf, "CLIENT_LOG_DIR");
                config_setting_set_string(LOGS, CLIENT_LOG_DIR);
                config_write_file(&conf, CLIENT_CONFIG_FILE);
                break;

            case 'd':
                CLIENT_TORRENTS = strdup((const char*)optarg);
                if (!strcmp(CLIENT_TORRENTS, "")) break;
                config_setting_t *TORRENTS = config_lookup(&conf, "CLIENT_TORRENTS");
                config_setting_set_string(TORRENTS, CLIENT_TORRENTS);
                config_write_file(&conf, CLIENT_CONFIG_FILE);
                break;

            case 'e':
                CLIENT_DOWNLOAD = strdup((const char*)optarg);
                if (!strcmp(CLIENT_DOWNLOAD, "")) {
                    char cwd[FILENAME_MAX];
                    if (!getcwd(cwd, sizeof(cwd))) {
                        fprintf(stderr, "Could not get the current work directory :: %s\n", strerror(errno));
                        return -1;
                    }
                }          
                config_setting_t *DOWNLOAD = config_lookup(&conf, "CLIENT_DOWNLOAD");
                config_setting_set_string(DOWNLOAD, CLIENT_DOWNLOAD);
                config_write_file(&conf, CLIENT_CONFIG_FILE);
                break;

            case 'h':
                help(argv[0]);

            default:
                break;
        }
    }

    struct Node *seeder = node_create(CLIENT_IP, CLIENT_PORT);    // our client seeder, but can be other seeder on the network

    struct Node *cserver = node_create(CSERVER_IP, CSERVER_PORT); // centrar server node
    char *json = NULL;

    // if (!strcmp(argv[1], "_start_")) {
    //     seeder_start(seeder);
    //     err = 1; goto clean;
    // }

    if (is_a_torrent(filename)) {
        if (jsonReadFile(filename, seeder, 0) < 0) {
            fprintf(stderr, "Could not load the json file_info content of the file %s\n", filename);
            err = -1; goto clean;
        }

        if (!(json = nodeSerialize(seeder, SERR_NET))) {
            fprintf(stderr, "Could not load the json addr info content of the file %s\n", filename);
            err = -1; goto clean;
        }
    } else {
        if (file_info_load(filename, seeder->fileinfo) < 0) {
            fprintf(stderr, "Could not load the file %s\n", filename);
            err = -1; goto clean;
        }

        if (create_symlink(filename, seeder->fileinfo->file_name) < 0) {
            err = -1; goto clean;
        }
    
        if (jsonWriteFile(&filename, seeder, 0) < 0) {
            fprintf(stderr, "Could not create the torrent\n");
            err = -1; goto clean;
        }

        printf("Created %s successfully!\n", filename);

        if (!(json = nodeSerialize(seeder, SERR_NET))) {
            fprintf(stderr, "Was not possible to deserialize the node!\n");
            free(filename);
            err = -1; goto clean;
        }

        free(filename);
    }


    int nodefd = connectton(cserver, json, strlen(json)+1);
    

    char buf[512];
    if (recvfromn(nodefd, buf, 512)) {
        err = -1; goto clean;
    }
    
    if (!(seeder = nodeDeserialize(buf, NULL, SERR_NET))) {
        err = -1; goto clean;
    }

    if (seeder->addr.sin_port == 0) {
        err = -1; goto clean;
    }

    printf("IP: %s\nPORT: %d\n", inet_ntoa(seeder->addr.sin_addr), ntohs(seeder->addr.sin_port));

    close(nodefd);

    nodefd = connectton(seeder, json, strlen(json)+1);
    char data[1024];
    if (recvfromn(nodefd, data, sizeof(data)) < 0) {
        err = -1; goto clean;
    }
    savefile(seeder->fileinfo->file_name, data, 1024);

clean:
    close(nodefd);
    node_destroy(seeder);
    node_destroy(cserver);
    free(json);
    config_destroy(&conf);

    // free((char*)CLIENT_IP);
    // free((char*)CLIENT_LOG_DIR);
    // free((char*)CLIENT_TORRENTS);
    // free((char*)CLIENT_DOWNLOAD);

    // free((char*)CSERVER_IP);

    return err;
}