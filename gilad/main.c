#include <stdio.h>
#include <sys/select.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/ip.h>

#include "tun_interface.h"
#include "icmp_wrap.h"

#define TUN_IF_NAME ("tun0")
#define TIMOUT_SEC (500)
#define MAX_DATA_SIZE (1500)
#define IP_DEFAULT_HDR_LEN (5)
#define ICMP_PROTOCOL (1)

#define EXIT_FUNC(V) do { \
    err = V; \
    goto cleanup; \
} while (0)

/**
 * Wrapps a given IP packet in an ICMP cover, and sends it.
 * 
 * @param sock_fd   [IN]    a socket fd (returned from calling socket(AF_INET, SOCK_RAW, IPPROTO_RAW))
 * @param packet    [IN]    A buffer containing the IP packet (including the header)
 * @param size      [IN]    The size of the buffer 
 * 
 * @return On success 0, on failure -1.
 */
int wrap_and_send(int sock_fd, const void* packet, size_t size) {
    size_t wrapped_size = 0;
    void* packet_to_send = NULL;
    struct iphdr* wrap_ip_header = NULL;
    struct sockaddr_in addr = {0};
    int ret = -1;
    int err = 0;
    ssize_t sent = -1;

    wrapped_size = sizeof(struct iphdr) + icmp_wrapped_packet_size(size);
    packet_to_send = malloc(wrapped_size);
    if (!packet_to_send) EXIT_FUNC(-1);
    
    // Sets up the first level IP information
    memcpy(packet_to_send, packet, sizeof(struct iphdr));
    wrap_ip_header = ((struct iphdr*) packet_to_send);
    wrap_ip_header->ihl = IP_DEFAULT_HDR_LEN;
    wrap_ip_header->tot_len  = wrapped_size;
    wrap_ip_header->protocol = ICMP_PROTOCOL;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = wrap_ip_header->daddr;

    // Sets up the ICMP information
    ret = icmp_wrap_packet(false, packet, size, (void*) (((char*) packet_to_send) + sizeof(struct iphdr)));
    if (ret < 0) EXIT_FUNC(-1);
    // TODO: Calculate and set the IP checksum

    // Sends the packet
    sent = sendto(sock_fd, packet_to_send, wrapped_size, 0, (struct sockaddr*) &addr, sizeof(addr));
    if (sent < 0 || sent < wrapped_size) EXIT_FUNC(-1);

cleanup:
    free(packet_to_send);
    return err;
}


int main() {
    int fd = -1;
    int sock_fd = -1;
    fd_set readfs = {0};
    struct timeval timeout = {0};
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
        printf("Recived packet of size %ld\n", data_size);

        // Wraps in an ICMP packet and sends it
        if(wrap_and_send(sock_fd, data, data_size) >= 0) {
            printf("Sent wrapped packet\n");
        }
    }
    
    return 0;
}