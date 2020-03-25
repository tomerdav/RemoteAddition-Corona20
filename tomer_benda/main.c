#include <stdio.h>

#include "tun.h"

int main() {
    int tun_fd = -1;

    tun_fd = tun_alloc("moms_spaghetti");

    tun_close(tun_fd);

    printf("Cyber\n");
    return 0;
}
