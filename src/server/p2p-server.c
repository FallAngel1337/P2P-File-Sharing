/**
 * p2p-server.c - P2P server
 * Author: FallAngel
 * Date: 2021-08-14
 * 
 * Copyright (c) 2021
 */
/**
 * Will answer the client with
 * the file owners.
*/

// Global headers
#include <node/node.h>
#include <node/serr/node_serr.h>

// Server headers
#include <config/server/config.h>
#include <net/rtable/rtable.h>
#include <net/conn/conn.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libconfig.h>
#include <errno.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BACKLOG_MAX 5

int main(int argc, char **argv) 
{
    config_t conf;

    const char *CSERVER_IP;
    unsigned CSERVER_PORT;
    unsigned RTABLE_SIZE;

    config_init(&conf);

    if (config_read_file(&conf, SERVER_CONFIG_FILE) == CONFIG_FALSE) {
        fprintf(stderr, "Can't load the configuration file %s :: %s\n", SERVER_CONFIG_FILE, config_error_text(&conf));
        config_destroy(&conf);
        return -1;
    }
    
    config_lookup_string(&conf, "CSERVER_IP", &CSERVER_IP);
    config_lookup_int(&conf, "CSERVER_PORT", &CSERVER_PORT);
    config_lookup_int(&conf, "RTABLE_SIZE", &RTABLE_SIZE);

    struct rtable **table = table_create(RTABLE_SIZE);
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int sock;

    memset(&addr, 0, len);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(CSERVER_IP);
    addr.sin_port = htons(CSERVER_PORT);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "socket error: %s\n", strerror(errno));
        config_destroy(&conf);
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        fprintf(stderr, "setsockopt error: %s\n", strerror(errno));
        config_destroy(&conf);
        close(sock);
        return -1;
    }

    if (bind(sock, (struct sockaddr*)&addr, len) < 0) {
        fprintf(stderr, "bind error: %s\n", strerror(errno));
        config_destroy(&conf);
        close(sock);
        return -1;
    }

    if (listen(sock, BACKLOG_MAX) < 0) {
        fprintf(stderr, "listen error: %s\n", strerror(errno));
        config_destroy(&conf);
        close(sock);
        return -1;
    }

    printf("Running at %s:%d\n", CSERVER_IP, CSERVER_PORT);

    while (1) {
        struct sockaddr_in client_addr;
        int connfd = accept(sock, (struct sockaddr*)&client_addr, &len);
        struct Node *client_node = NULL;

        if (recvfromc(connfd, &client_node, SERR_NET) < 0) {
            fprintf(stderr, "recvfromc failed %s\n", strerror(errno));
            close(connfd);
            break;
        }

        char *nodeserr;

        struct Node *seeder_addr = table_lookup(table, client_node->fileinfo, RTABLE_SIZE);
        if (sendtoc(connfd, seeder_addr) < 0) {
            fprintf(stderr, "Was not possible to send json! error: %s\n", strerror(errno));
            break;
        }

        if (table_insert(table, client_node, RTABLE_SIZE) < 0) {
            fprintf(stderr, "Was not possible to insert node\n");
            break;
        }

        show_table(table, RTABLE_SIZE);
    }

    config_destroy(&conf);
    free(table);
    close(sock);
    return 0;
}