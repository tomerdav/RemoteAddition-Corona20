#include "icmp_cover.h"

#define ICMP_HEADER_SIZE 8

int build_icmp_cover(char* original_buffer, char** new_buffer, int original_size, bool is_reply) {
    int new_size = get_covered_icmp_size(original_size);
    *new_buffer = malloc(new_size);

    if (!(*new_buffer)) {
        return -1;
    }

    // build the header of the covered packet
    ICMP_HEADER* header = *new_buffer;
    header->type = is_reply ? (uint8_t)0 : (uint8_t)8;
    header->code = (uint8_t)0;
    header->checksum = (uint16_t)0;
    header->identifier = (uint16_t)0;
    header->sequence_number = (uint16_t)0;
    
    // add the data to the covered packet
    memcpy((*new_buffer) + ICMP_HEADER_SIZE, buffer, original_size);  

    return new_size;   
}

int get_covered_icmp_size(int size) {
    return (ICMP_HEADER_SIZE + size);
}

int send_icmp_covered_packet(char* buffer, int size, char* dest) {
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (fd == -1) {
        return -1;
    }

    if (sendto(fd, &req, req_size, 0, res->ai_addr, res->ai_addrlen) ==-1) {
        return -1;
    }


    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(dest);

    struct iovec iov[1] = {0};
    iov[0].iov_base = &req;
    iov[0].iov_len = req_size;
    
    struct msghdr message;
    message.msg_name = &server;
    message.msg_namelen = sizeof(server);
    message.msg_iov = iov;
    message.msg_iovlen = 1;
    message.msg_control = 0;
    message.msg_controllen = 0;

    if (sendmsg(fd, &message, 0) == -1) {
        return -1;
    }
    
    return size;
}

