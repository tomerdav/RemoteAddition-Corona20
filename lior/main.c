#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "tun.h"

#define PATH_MAX_SIZE 100
#define TUN_NAME "tun1"
#define SCRIPT "bash route_tun.sh"

char tun_name[IFNAMSIZ];

int main() {
    int fd = -1, err = 0, sock = -1;
  
    printf("Starting icmp tunnel...\n");
    strncpy(tun_name, TUN_NAME, strlen(TUN_NAME) + 1);
    fd = tun_alloc(tun_name);
    
    if (fd < 0) {
        return fd;
    }
    
    if (WEXITSTATUS(system(SCRIPT)) == 0) {
        printf("Script ran successfully\n");
    } else {
        printf("Error occured while running script\n");
        err -1;
        goto clean;
    }
    
    //open icmp socket
    
        

    while(1) {
        // read packet from tun
        // cover packet with icmp header
        // send packet
    }

clean:
    tun_close(fd);

    return err;
}
