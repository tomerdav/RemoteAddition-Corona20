#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "tun.h"

int tun_alloc(char *dev) {
    struct ifreq ifr;
    int tun_fd, err;
    char *path = "/dev/net/tun";

    tun_fd = open(path, O_RDWR);

    if(tun_fd < 0) {
        printf("Error open file\n");
        return tun_fd;
    }

    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;

    if(dev != NULL && *dev != '\0') {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if ((err = ioctl(tun_fd, TUNSETIFF, &ifr)) < 0) {
        close(tun_fd);
        printf("Error creating the device\n");
        return err;
    }

    strcpy(dev, ifr.ifr_name);

    return tun_fd;
}

void tun_close(int tun_fd) {
    if (tun_fd >= 0) {
        close(tun_fd);
    }
}

int tun_write(int tun_fd, const char *buf, int size) {
    int bytes_written = write(tun_fd, buf, size);
    if (bytes_written < size) {
        printf("Could not write all the bytes to the tunnel!\n");
    }
    return bytes_written;
}

int tun_read(int tun_fd, char *buf, int size) {
    int bytes_read = read(tun_fd, buf, size);
    if (bytes_read < size) {
        printf("Could not read all the bytes from the tunnel!\n");
    }
    return bytes_read;
}


