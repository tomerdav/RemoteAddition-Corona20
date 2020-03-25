#include <string.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include "tunnel.h"

#define TUN_NAME "tun33"
#define DEVICE_PATH "/dev/net/tun"

int main() {
    char c[100];
    int fd = tun_alloc(TUN_NAME);
    printf("%d\n", fd);
    scanf("%s", c);
    tun_close(fd);
    return 0;
}

int tun_alloc(char* dev_name) {
    struct ifreq ifr;
    int fd, err;

    // open the device
    if((fd = open(DEVICE_PATH, O_RDWR)) < 0) {
        return fd;
    }

    // preparation of the struct ifr
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN;

    if (*dev_name) {
        strncpy(ifr.ifr_name, dev_name, IFNAMSIZ);
    }

    // try to create the new device
    if((err = ioctl(fd, TUNSETIFF, (void*)&ifr)) < 0) {
        close(fd);
        return err;
    }

    return fd;
}



void tun_close(int fd) {
    close(fd);
}

void tun_write() {
}

void tun_read() {
}
