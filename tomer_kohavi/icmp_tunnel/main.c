#include <stdio.h>
#include <stdlib.h>

#include "icmp_tun.c"

int main() {
    int tun_fd;
    char buffer[BUF_SIZE];
    int sock_fd;
    struct sockaddr_in remote;

    tun_fd = tun_alloc(DEV_NAME);
    
    if (-1 == tun_fd) {
        perror("tun_alloc()");
        return -1;
    }

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == sock_fd) {
        perror("socket()");
        return -1;
    }

    

    while(1) {
        if (-1 == tun_read(tun_fd, (void *)buffer)) {
            tun_close(tun_fd);
            return -1;
        }

        printf("%p\n", buffer);
    }

    return 0;
}
