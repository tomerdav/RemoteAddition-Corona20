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

#define DEVICE_PATH "/dev/net/tun"

int tun_alloc(char* dev_name) {
    struct ifreq ifr;
    int fd, err = 0;

    // open the device
    if((fd = open(DEVICE_PATH, O_RDWR)) == -1) {
        return -1;
    }

    // preparation of the struct ifr
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN;

    if (*dev_name) {
        strncpy(ifr.ifr_name, dev_name, IFNAMSIZ);
    }

    // try to create the new device
    if((err = ioctl(fd, TUNSETIFF, (void*)&ifr)) == -1) {
        close(fd);
        return -1;
    }

    return fd;
}



void tun_close(int fd) {
    close(fd);
}

int tun_write(int fd, char* buffer, size_t size) {
    int err = 0;
    ssize_t num_written = 0;
    ssize_t curr_num_written = 0;

    while (num_written < size) {
        curr_num_written = write(fd, buffer + num_written, size - num_written);
        
        if (curr_num_written == -1) {
            return -1;
        }

        num_written += curr_num_written;        
    }

    return err;
}

int tun_read(int fd, char* buffer, size_t size) {
    return read(fd, buffer, size);
}

