#include <stdio.h>
#include "tunnel.h"

#define TUN_NAME "tun33"

int main() {
    char c[100];
    int fd = tun_alloc(TUN_NAME);

    scanf("%s", c);
    tun_close(fd);

    return 0;
}

