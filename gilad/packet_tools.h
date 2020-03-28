#include <stdint.h> 

/**
 * Wrapps a given IP packet in an ICMP cover, and sends it.
 * 
 * @param sock_fd   [IN]    a socket fd (returned from calling socket(AF_INET, SOCK_RAW, IPPROTO_RAW))
 * @param packet    [IN]    A buffer containing the IP packet (including the header)
 * @param size      [IN]    The size of the buffer 
 * @param dest_addr [IN]    The IPv4 address to send the wrapped packet to (if x.x.x.x format)
 * 
 * @return On success 0, on failure -1.
 */
int wrap_and_send(int sock_fd, const void* packet, size_t size, char* dest_addr);