#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>

/* Sets the src ip */
void set_src_ip(struct ip *ip);

/* Sets the dst ip */
void set_dst_ip(struct ip *ip, const char *dst_name);

/* Creates a raw socket */
int create_socket();

/* Creates a icmp packet with no data */
void create_icmp_packet(struct icmp *icmp, struct ip *ip, char *send_bufi, int type);

/* Sends an icmp packet */
int send_packet(int sock_fd, struct icmp *icmp, struct ip *ip, char *send_buf, const char *data, int data_len);

/* Checks if the packet is from an icmp tunnel */
int is_icmp_tunnel(const char *packet);

/* One's Complement checksum algorithm */
unsigned short cksum(unsigned short *addr, size_t len);
