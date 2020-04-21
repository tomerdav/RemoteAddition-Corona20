#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <linux/ip.h>
#include "icmp_cover.h"
#include "defs.h"

#define ICMP_HEADER_SIZE (8)
#define IP_HEADER_SIZE (20)
#define ICMP_PROTOCOL_NUMBER ((uint8_t)1)

uint16_t get_icmp_checksum(char* buffer, int size)
{
    uint16_t* header = (uint16_t*)buffer;
	uint16_t ret = 0;
	uint32_t sum = 0;
	uint16_t odd_byte;
	
	while (size > 1) {
		sum += *header++;
		size -= 2;
	}
	
	if (size == 1) {
		*(uint8_t*)(&odd_byte) = *(uint8_t*)header;
		sum += odd_byte;
	}
	
	sum =  (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	ret =  ~sum;
	
	return ret; 
}

int socket_read(int fd, char* buffer, int size) {
    return recvfrom(fd, buffer, size, 0, NULL, 0);
}

bool is_icmp_packet(char* buffer, int size) {
    struct iphdr* ip_header = (struct iphdr*)(buffer);
    return (ip_header->protocol == ICMP_PROTOCOL_NUMBER);
}

int remove_icmp_cover(char* original_buffer, char** new_buffer, int original_size, uint32_t* dest) {   
    if (!is_icmp_packet(original_buffer, original_size)) {
        return original_size;
    }

    int new_size = original_size - ICMP_HEADER_SIZE - IP_HEADER_SIZE;
    *new_buffer = malloc(new_size);

    if(!(*new_buffer)) {
        return ERROR_CODE;    
    }

    memcpy(*new_buffer, original_buffer + ICMP_HEADER_SIZE + IP_HEADER_SIZE, new_size);
    
    struct iphdr* ip_header = (struct iphdr*)(*new_buffer);
    // manipulate source addr
    ip_header->saddr = inet_addr(SERVER_ADDR);
 
    *dest = ip_header->daddr;

    return new_size;
}

int build_icmp_cover(char* original_buffer, char** new_buffer, int original_size, bool is_reply) {
    int new_size = get_covered_icmp_size(original_size);
    *new_buffer = malloc(new_size);

    if (!(*new_buffer)) {
        return ERROR_CODE;
    }

    // build the header of the covered packet
    ICMP_HEADER* header = (ICMP_HEADER*)*new_buffer;
    header->type = is_reply ? (uint8_t)0 : (uint8_t)8;
    header->code = (uint8_t)0;
    header->checksum = (uint16_t)0;
    header->identifier = (uint16_t)0;
    header->sequence_number = (uint16_t)0;

    // add the data to the covered packet
    memcpy((*new_buffer) + ICMP_HEADER_SIZE, original_buffer, original_size);  
    header->checksum = get_icmp_checksum(*new_buffer, new_size);

    return new_size;   
}

int get_covered_icmp_size(int size) {
    return (ICMP_HEADER_SIZE + size);
}

int send_socket_msg(int fd, char* buffer, int size, uint32_t dest) {
    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = dest;    

    struct iovec iov[1] = {0};
    iov[0].iov_base = buffer;
    iov[0].iov_len = size;
    
    struct msghdr message = {0};
    message.msg_name = &server;
    message.msg_namelen = sizeof(server);
    message.msg_iov = iov;
    message.msg_iovlen = 1;
    message.msg_control = 0;
    message.msg_controllen = 0;

    if (sendmsg(fd, &message, 0) == ERROR_CODE) {
        close(fd);
        return ERROR_CODE;
    }

    close(fd);
    return size;
}

int send_icmp_uncovered_packet(char* buffer, int size, uint32_t dest) {
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (fd == ERROR_CODE) {
        return ERROR_CODE;
    }

    return send_socket_msg(fd, buffer, size, dest);
}

int send_icmp_covered_packet(char* buffer, int size, char* dest) {
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (fd == ERROR_CODE) {
        return ERROR_CODE;
    }

    return send_socket_msg(fd, buffer, size, inet_addr(dest));
}

