#include <stdio.h>

#include "tun_interface.h"

int main() {
    int fd = -1;

    fd = tun_alloc("tun0");
    printf("fd = %d\n", fd);
    while (1) {}

    return 0;
}