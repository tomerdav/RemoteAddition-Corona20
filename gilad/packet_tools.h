#include <stdint.h> 

/**
 * Wraps a given IP packet in an ICMP cover, and sends it.
 * 
 * @param sock_fd   [IN]    a socket fd (returned from calling socket(AF_INET, SOCK_RAW, IPPROTO_RAW))
 * @param packet    [IN]    A buffer containing the IP packet (including the header)
 * @param size      [IN]    The size of the buffer 
 * @param dest_addr [IN]    The IPv4 address to send the wrapped packet to (if x.x.x.x format)
 * 
 * @return On success 0, on failure -1.
 */
int wrap_and_send(int sock_fd, const void* packet, size_t size, char* dest_addr);


/**
 * Unwraps a given IP packet that is inside an ICMP cover, and sends it to its destination.
 * 
 * @param sock_fd   [IN]    a socket fd (returned from calling socket(AF_INET, SOCK_RAW, IPPROTO_RAW))
 * @param packet    [IN]    A buffer containing covered the IP packet (including the ICMP and IP headers)
 * @param size      [IN]    The size of the buffer
 * 
 * @return On success 0, on failure -1.
 */
int unwrap_and_send(int sock_fd, void* packet, size_t size);
