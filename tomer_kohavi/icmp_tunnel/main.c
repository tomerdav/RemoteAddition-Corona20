#include <stdio.h>
#include <stdlib.h>

#include "icmp_tun.c"

int main() {
    int tun_fd;
    void* buffer;

    tun_fd = tun_alloc(DEV_NAME);
    
    if (tun_fd == -1) {
        return -1;
    }

    buffer = malloc(BUF_SIZE);

    if (!buffer) {
        tun_close(tun_fd);
        return -1;
    }

    while(1) {
        if (-1 == tun_read(tun_fd, buffer)) {
            tun_close(tun_fd);
            return -1;
        }

        printf("%p\n", buffer);
    }

    return 0;
}
