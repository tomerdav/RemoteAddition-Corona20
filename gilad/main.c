#include <stdio.h>
#include <sys/select.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "tun_interface.h"
#include "packet_tools.h"

#define TUN_IF_NAME ("tun0")
#define TIMEOUT_SEC (500)
#define MAX_DATA_SIZE (1500)
#define PROXY_SERVER_ADDR ("127.0.0.1")


/**
 * Wait for file to be ready for reading.
 * 
 * @param fd        [IN]    The file's descriptor
 * @param timeout   [IN]    The maximal waiting period (in seconds)
 * 
 * @return Ready: ret > 0, Timeout: ret == 0, Error: ret < 0
 */
int wait_for_read(int fd, time_t timeout) {
    fd_set readfs = {0};
    struct timeval tv_timeout = {0};
    FD_ZERO(&readfs);
    FD_SET(fd, &readfs);
    tv_timeout.tv_sec = timeout;
    tv_timeout.tv_usec = 0;
    return select(fd + 1, &readfs, NULL, NULL, &tv_timeout);
}


int main() {
    int fd = -1;
    int sock_fd = -1;
    int ret = 0;
    char data[MAX_DATA_SIZE] = {0};
    ssize_t data_size = 0;

    // Creates the device
    fd = tun_alloc(TUN_IF_NAME);
    if (fd < 0) {
        printf("Failed to create the interface %s\n", TUN_IF_NAME);
        return 1;
    }
    printf("Created the interface %s\n", TUN_IF_NAME);

    // Creates the socket
    sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock_fd < 0) {
        printf("Failed to create socket\n");
        return 1;
    }

    while (true) {
        // Waits for data from the interface
        ret = wait_for_read(fd, TIMEOUT_SEC);
        if (ret == 0) {
            printf("Reached timout (%d seconds)\n", TIMEOUT_SEC);
        }
        if (ret <= 0) break;

        // Reads from the interface
        data_size = tun_read(fd, (void*) data, MAX_DATA_SIZE);
        if (data_size <= 0) break;
        printf("Recived packet of size %ld\n", data_size);

        // Wraps in an ICMP packet and sends it
        if(wrap_and_send(sock_fd, data, data_size, PROXY_SERVER_ADDR) >= 0) {
            printf("Sent wrapped packet\n");
        }
    }
    
    return 0;
}