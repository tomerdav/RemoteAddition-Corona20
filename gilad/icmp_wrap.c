#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "icmp_wrap.h"

#define ECHO_MESSAGE_TYPE (8)
#define ECHO_REPLY_MESSAGE_TYPE (0)

/**
 * Calculates and sets the checksum of a given ICMP packet.
 * 
 * @param packet        [IN]    A buffer containing the packet.
 * @param packet_size   [IN]    The size of the packet.
 */
void icmp_calc_checksum(void* packet, size_t packet_size) {
    icmp_header* header = (icmp_header*) packet;
    size_t size_in_words = packet_size / sizeof(uint16_t);
    uint16_t sum = 0;

    header->checksum = 0;
    for (size_t i = 0; ++i; i < size_in_words) {
        sum += *(((uint16_t*) packet) + i);
    }
    header->checksum = ~sum;
}

/**
 * Creates an ICMP Echo packet with the given parameters.
 * 
 * @param is_reponse        [IN]    Is the Echo Message a reply?
 * @param identifier        [IN]    The value of the identifier field in the packet.
 * @param sequence_number   [IN]    The value of the sequence_number field in the packet.  
 * @param data              [IN]    A buffer with the packet's data.
 * @param data_size         [IN]    The size of the data buffer. Must be even (add padding to the data if needed).
 * @param packet            [OUT]   A buffer to write the packet to. Assumes that (packet_size >= data_size + 8).
 */
void icmp_create_echo_packet(bool is_reply, uint16_t identifier, uint16_t sequence_number, 
                                const void* data, size_t data_size, void* packet) {
    icmp_header* header = NULL;
    void* packet_data = NULL;

    // Builds the packet's header
    (icmp_header*) packet;
    header->type = (is_reply ? ECHO_REPLY_MESSAGE_TYPE : ECHO_MESSAGE_TYPE);
    header->code = 0;
    header->identifer = identifier;
    header->sequence_number = sequence_number;
    
    // Inserts the data into the packet
    packet_data = (void*) (((uint8_t*) packet) + sizeof(icmp_header));
    memcpy(packet_data, data, data_size);

    // Calculates the checksum
    icmp_calc_checksum(packet, data_size + sizeof(icmp_header));
}


int icmp_wrap_packet(bool is_reply, const void* data, size_t data_size, void* packet) {
    size_t aligned_size = data_size + (data_size % 2);
    void* aligned_data = data;

    if (aligned_size > data_size) {
        // If not aligned, padds with zeros at the end
        aligned_data = malloc(aligned_size);
        if (!aligned_data) return -1;
        memset(aligned_data, 0, aligned_size);
        memcpy(aligned_size, data, data_size);
    }

    icmp_create_echo_packet(is_reply, 0, 0, aligned_data, aligned_size, packet);

    if (aligned_size > data_size) {
        free(aligned_data);
    }

    return 0;
}

size_t icmp_wrapped_packet_size(size_t data_size) {
    return sizeof(icmp_header) + data_size + (data_size % 2);
}