#include <stdio.h>
#include <sys/select.h>
#include <stdbool.h>
#include <stdlib.h>

#include "tun_interface.h"
#include "icmp_wrap.h"

#define TUN_IF_NAME ("tun0")
#define TIMOUT_SEC (500)
#define MAX_DATA_SIZE (1500)


int main() {
    int fd = -1;
    fd_set readfs = {0};
    struct timeval timeout = {0};
    int ret = 0;
    char data[MAX_DATA_SIZE] = {0};
    ssize_t data_size = 0;
    size_t new_data_size = 0;
    void* wrapped_packet = NULL;

    // Creates the device
    fd = tun_alloc(TUN_IF_NAME);
    if (fd < 0) {
        printf("Failed to create the interface %s\n", TUN_IF_NAME);
        return 1;
    }
    printf("Created the interface %s\n", TUN_IF_NAME);

    while (true) {
        // Waits for data from the interface
        FD_ZERO(&readfs);
        FD_SET(fd, &readfs);
        timeout.tv_sec = TIMOUT_SEC;
        timeout.tv_usec = 0;
        ret = select(fd + 1, &readfs, NULL, NULL, &timeout);
        if (ret == 0) {
            printf("Reached timout (%d seconds)\n", TIMOUT_SEC);
        }
        if (ret <= 0) break;

        // Reads from the interface
        data_size = tun_read(fd, (void*) data, MAX_DATA_SIZE);
        if (data_size <= 0) break;

        // Wraps in an ICMP packet and writes to the interface
        new_data_size = icmp_wrapped_packet_size(data_size);
        wrapped_packet = malloc(new_data_size);
        if (!wrapped_packet) break;
        icmp_wrap_packet(false, (void*) data, data_size, wrapped_packet);
        // TODO: Send the packet
    }
    
    return 0;
}