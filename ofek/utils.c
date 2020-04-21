#include <stdlib.h>
#include "utils.h"
#include "defs.h"

int wait_for_tun_reading(int fd, char* buffer, int size) {
    int err = 0;    
    fd_set read_fds = {0};     
    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);

    err = select(fd + 1, &read_fds, NULL, NULL, NULL);
    
    if (err <= 0) {
        return ERROR_CODE;
    }
    
    return tun_read(fd, buffer, size);    
}

