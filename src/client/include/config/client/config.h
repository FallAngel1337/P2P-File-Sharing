/**
 * config.h - Client configuration header
 * Author: FallAngel
 * Date: 2021-08-13
 * 
 * Copyright (c) 2021
 */

#ifndef _CONFIG_H
#define _CONFIG_H

#define CLIENT_CONFIG_FILE "/etc/p2pd/client.conf"

// not sure if this is a good idea, but... let's pretend it is
extern const char *CLIENT_IP;
extern unsigned CLIENT_PORT;
extern const char *CLIENT_LOG_DIR;
extern const char *CLIENT_TORRENTS;
extern const char *CLIENT_DOWNLOAD;

extern const char *CSERVER_IP;
extern unsigned CSERVER_PORT;

#endif /* _CONFIG_H */