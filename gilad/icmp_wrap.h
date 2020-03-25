#include <stdint.h>

typedef struct {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t identifer;
    uint16_t sequence_number;
} icmp_header;

/**
 * Wrapps a given packet with an ICMP packet.
 * 
 * @param is_reply      [IN]    Should the wrap be ICMP Echo Message reply?
 * @param data          [IN]    A buffer with the packet's data.
 * @param data_size     [IN]    The size of the buffer.
 * 
 * @return A pointer to the new packet. NEED TO BE FREED!
 */
void* icmp_wrap_packet(bool is_reply, const void* data, size_t data_size);