#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include "tun_interface.h"

#define INVALID_FD (-1)


int tun_alloc(char* device_name) {
    struct ifreq if_request = {0};
    int fd = INVALID_FD;
    int err = 0;

    fd = open("/dev/net/tun", O_RDWR);
    if (fd < 0) {
        return INVALID_FD;
    }

    if_request.ifr_flags = IFF_TUN;
    strncpy(if_request.ifr_name, device_name, IFNAMSIZ);
    
    err = ioctl(fd, TUNSETIFF, (void *) &if_request);
    if (err < 0) {
        close(fd);
        return INVALID_FD;
    }

    return fd;
}


int tun_close(int fd) {
    return close(fd);
}


ssize_t tun_write(int fd, const void* buffer, size_t count) {
    return write(fd, buffer, count);
}


ssize_t tun_read(int fd, void* buffer, size_t count) {
    return read(fd, buffer, count);
}