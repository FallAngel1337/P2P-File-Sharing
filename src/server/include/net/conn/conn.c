#include "../../../../include/node/serr/node_serr.h"
#include "conn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

// Recieve the json from client and deserialize
int recvfromc(int _fd, struct Node *_node)
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

    if (!nodeDeserialize(_buf, _node)) {
        fprintf(stderr, "recvfromc failed to deserialize\n");
        close(_fd); free(_buf);
        return -1;
    }


    free(_buf);
    return 0;
}

// Send to client the address of the file owner
int sendtoc(int _fd, struct sockaddr_in *_addr)
{

}