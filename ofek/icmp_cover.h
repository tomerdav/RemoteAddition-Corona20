#ifndef _ICMP_COVER_DEFINE_
#define _ICMP_COVER_DEFINE_

typedef struct {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t identifier;
    uint16_t sequence_number;
} ICMP_HEADER;

int get_covered_icmp_size(int size);
int build_icmp_cover(char* original_buffer, char** new_buffer, int original_size, bool is_reply);
int send_icmp_covered_packet(char* buffer, int size, char* dest);

#endif
