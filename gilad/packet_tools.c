#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <linux/icmp.h>
#include <netinet/in.h>
#include <linux/ip.h>
#include <arpa/inet.h>
#include "packet_tools.h"

#define IP_DEFAULT_HDR_LEN (5)
#define ICMP_PROTOCOL (1)

#define EXIT_FUNC(V) do { \
    err = V; \
    goto cleanup; \
} while (0)


/**
 * Calculates the checksum of a given packet (works for both ICMP and IP header).
 * It is important that the checksum field in the packet will be set to 0.
 * 
 * @param buffer    [IN]    A buffer containing the packet.
 * @param size      [IN]    The size of the packet.
 */
uint16_t calc_checksum(void* buffer, size_t size) {
    size_t size_in_words = size / sizeof(uint16_t);
    uint32_t sum = 0;

    // Sums up
    for (size_t i = 0; i < size_in_words; ++i) {
        sum += *(((uint16_t*) buffer) + i);
    }

    // Adds the carry and negate
    return ~(sum + (sum >> 16));
}


/**
 * Calculates the size of a given packet after it will be warpped.
 * 
 * @param data_size     [IN]    The original packet size
 * 
 * @return The wrapped packet size.
 */
size_t wrapped_packet_size(size_t data_size) {
    return sizeof(struct iphdr) + sizeof(struct icmphdr) + data_size + (data_size % 2);
}


/**
 * Calculates the size of a given packet after it will be unwarpped.
 * 
 * @param data_size     [IN]    The original packet size
 * 
 * @return The unwrapped packet size.
 */
size_t unwrapped_packet_size(size_t data_size) {
    return data_size - (sizeof(struct iphdr) + sizeof(struct icmphdr));
}


/**
 * Adds a given amount to a pointer.
 * 
 * @param pointer   [IN]    The pointer
 * @param amount    [IN]    The amount (in bytes) to add
 * 
 * @return pointer + amount
 */
void* pointer_add(void* pointer, size_t amount) {
    return (void*) (((char*) pointer) + amount);
}


/**
 * Wraps a given IP packet with an ICMP packet.
 * 
 * @param data          [IN]    A buffer with the packet's data.
 * @param data_size     [IN]    The size of the buffer.
 * @param dest_addr     [IN]    The IP address to which the packet will be sent
 * @param is_reply      [IN]    Should the wrap be ICMP Echo Message reply?
 * @param packet        [OUT]   A buffer to write the warpped packet to.
 *                              Its size should be at least wrapped_packet_size(data_size).
 */
void icmp_wrap_packet(const void* data, size_t data_size, in_addr_t dest_addr, bool is_reply, void* packet) {
    struct iphdr* ip_header = NULL;
    struct icmphdr* icmp_header = NULL;
    void* packet_data = NULL;
    size_t total_size = wrapped_packet_size(data_size);

    memset(packet, 0, total_size);

    ip_header = ((struct iphdr*) packet);
    icmp_header = (struct icmphdr*) pointer_add(packet, sizeof(struct iphdr));
    packet_data = pointer_add(packet, sizeof(struct iphdr) + sizeof(struct icmphdr));

    // Builds the IP header
    memcpy(packet, data, sizeof(struct iphdr));
    ip_header->ihl = IP_DEFAULT_HDR_LEN;
    ip_header->tot_len  = total_size;
    ip_header->protocol = ICMP_PROTOCOL;
    ip_header->daddr = dest_addr;
    ip_header->check = 0;
    ip_header->check = calc_checksum(ip_header, sizeof(struct iphdr));

    // Builds the ICMP header
    memset(icmp_header, 0, sizeof(struct icmphdr));
    icmp_header->type = (is_reply ? ICMP_ECHOREPLY : ICMP_ECHO);
    
    // Inserts the data into the packet
    memcpy(packet_data, data, data_size);

    // Calculates the ICMP checksum
    icmp_header->checksum = calc_checksum(icmp_header, total_size - sizeof(struct iphdr));
}


int wrap_and_send(int sock_fd, const void* packet, size_t size, char* dest_addr) {
    size_t total_size = 0;
    void* packet_to_send = NULL;
    in_addr_t daddr = 0;
    struct sockaddr_in addr = {0};
    int err = 0;
    ssize_t sent = -1;

    // Warps the packet
    total_size = wrapped_packet_size(size);
    packet_to_send = malloc(total_size);
    if (!packet_to_send) EXIT_FUNC(-1);
    daddr = inet_addr(dest_addr);
    icmp_wrap_packet(packet, size, daddr, false, packet_to_send);
    
    // Sends the packet
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = daddr;
    sent = sendto(sock_fd, packet_to_send, total_size, 0, (struct sockaddr*) &addr, sizeof(addr));
    if (sent < 0 || sent < total_size) EXIT_FUNC(-1);

cleanup:
    free(packet_to_send);
    return err;
}


int unwrap_and_send(int sock_fd, void* packet, size_t size, char* src_addr) {
    struct sockaddr_in addr = {0};
    int err = 0;
    ssize_t sent = -1;
    size_t packet_size = unwrapped_packet_size(size);

    // Unwarps the packet
    void* packet_to_send = pointer_add(packet, sizeof(struct iphdr) + sizeof(struct icmphdr));
    struct iphdr* header = (struct iphdr*) packet_to_send;
    
    // Sends the packet
    header->saddr = inet_addr(src_addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = header->daddr;
    sent = sendto(sock_fd, packet_to_send, packet_size, 0, (struct sockaddr*) &addr, sizeof(addr));
    if (sent < 0 || sent < packet_size) EXIT_FUNC(-1);

cleanup:
    return err;
}