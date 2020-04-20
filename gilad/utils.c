#include <sys/select.h>
#include <stdlib.h>

#include "utils.h"

int wait_for_read(int fd, time_t timeout) {
    fd_set readfs = {0};
    struct timeval tv_timeout = {0};
    FD_ZERO(&readfs);
    FD_SET(fd, &readfs);
    tv_timeout.tv_sec = timeout;
    tv_timeout.tv_usec = 0;
    return select(fd + 1, &readfs, NULL, NULL, &tv_timeout);
}