#include <stdio.h>
#include "tunnel.h"

#define TUN_NAME "tun33"

int main() {
    char c[100];
    int fd = tun_alloc(TUN_NAME);

    read(fd, c, 30);
    printf("%d %d %d %d %d %d %d %d\n", c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);

    scanf("%s", c);
    tun_close(fd);

    return 0;
}

