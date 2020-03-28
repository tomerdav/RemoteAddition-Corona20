#include "icmp_tun.h"

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

int tun_alloc(char *dev)
{
    struct ifreq ifr;
    int fd, err;

	fd = open("/dev/net/tun", O_RDWR);

    if (fd < 0) {
        return -1;
	}

    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = IFF_TUN; 

    if (*dev) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    err = ioctl(fd, TUNSETIFF, (void *) &ifr);

    if (err < 0) {
        close(fd);
        return err;
    }

    return fd;
}

int tun_close(int fd) {
    return close(fd);
}

int tun_write(int fd, const void* buffer) {
    return write(fd, buffer, BUF_SIZE);
}

int tun_read(int fd, void* buffer) {
    return read(fd, buffer, BUF_SIZE);
}
