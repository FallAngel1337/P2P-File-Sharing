#include <node/serr/node_serr.h>
#include "conn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

// Recieve the json from client and deserialize
int recvfromc(int _fd, struct Node **_node, uint8_t _flags)
{
    char *_buf = calloc(1, 512);

    if (recv(_fd, _buf, 512, 0) < 0) {
        fprintf(stderr, "recv failed: %s\n", strerror(errno));
        close(_fd); free(_buf);
        return -1;
    }

#ifdef _DEBUG
    printf("%s\n", _buf);
#endif

    if (!(*_node = nodeDeserialize(_buf, *_node, _flags))) {
        fprintf(stderr, "recvfromc failed to deserialize\n");
        close(_fd); free(_buf);
        return -1;
    }

    free(_buf);
    return 0;
}

// Send to client the address of seeder
int sendtoc(int _fd, struct Node *_seeder)
{
    char *nodeserr;
    struct Node *_nullnode = node_create("127.0.0.1", 0);

    if (!_seeder) {
        fprintf(stderr, "Was not possible to find the node!\n");
        nodeserr = nodeSerialize(_nullnode, 0);
    } else {
        nodeserr = nodeSerialize(_seeder, SERR_NET);
    }

    if (send(_fd, nodeserr, strlen(nodeserr)+1, 0) < 0) return -1;

    node_destroy(_nullnode);
    return 0;
}