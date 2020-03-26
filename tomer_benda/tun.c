#include "tun.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <errno.h>

int tun_alloc(char *dev) {
    struct ifreq ifr;
    int fd, err;

    if((fd = open("/dev/net/tun", O_RDWR)) < 0){
        //return tun_alloc_old(dev);
        printf("error: %d\n", errno);
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN; 

    if(*dev) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
        close(fd);
        printf("error: %d\n", errno);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}
