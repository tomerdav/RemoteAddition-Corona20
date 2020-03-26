#include <stdint.h>

typedef struct {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t identifer;
    uint16_t sequence_number;
} icmp_header;

/**
 * Wraps a given packet with an ICMP packet.
 * 
 * @param is_reply      [IN]    Should the wrap be ICMP Echo Message reply?
 * @param data          [IN]    A buffer with the packet's data.
 * @param data_size     [IN]    The size of the buffer.
 * @param packet        [OUT]   A buffer to write the warpped packet to.
 *                              Its size should be at least icmp_wrapped_packet_size(data_size).
 * 
 * @return On success returns 0, else -1.
 */
int icmp_wrap_packet(bool is_reply, const void* data, size_t data_size, void* packet);

/**
 * Calculates the size of a given packet after it will be warpped.
 * 
 * @param data_size     [IN]    The original packet size
 * 
 * @return The wrapped packet size.
 */
size_t icmp_wrapped_packet_size(size_t data_size);