#include "icmp_packet.h"

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

const size_t ICMP_DATA_OFFSET = 28;
const char* MAGIC = "cOr";
const size_t MAGIC_LEN = 3;

void set_src_ip(struct ip *ip) {
    struct hostent *src_hp;
    char src_name[256];

    if (gethostname(src_name, sizeof(src_name)) < 0) {
        perror("gethostname() error");
        exit(EXIT_FAILURE);
    } else {
        if (NULL == (src_hp = gethostbyname(src_name))) {
            fprintf(stderr, "%s: Can't resolve, unknown source.\n", src_name);
            exit(EXIT_FAILURE);
        } else {
            ip->ip_src = (*(struct in_addr *)src_hp->h_addr);
        }
    }
}

void set_dst_ip(struct ip *ip, const char *dst_name) {
    if (-1 == (ip->ip_dst.s_addr = inet_addr(dst_name))) {
        fprintf(stderr, "%s: Can't resolve, unknown destination.\n", dst_name);
        exit(EXIT_FAILURE);
    }
}

int create_socket() {
    int sock_fd;
    int on = 1;
    /* Create RAW socket */
    if ((sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("sockdet() error");
        /* If something wrong, just exit */
        exit(EXIT_FAILURE);
    }

    /* socket options, tell the kernel we provide the IP structure */
    if (setsockopt(sock_fd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
        perror("setsockopt() for IP_HDRINCL error");
        exit(EXIT_FAILURE);
    }
    return sock_fd;
}

void create_icmp_packet(struct icmp *icmp, struct ip *ip, char *send_buf, int type) {
    /* IP structure, check the ip.h */
    ip->ip_v = 4;
    ip->ip_hl = 5;
    ip->ip_tos = 0;
    ip->ip_len = htons(sizeof(send_buf));
    ip->ip_id = htons(321);
    ip->ip_off = htons(0);
    ip->ip_ttl = 64;
    ip->ip_p = IPPROTO_ICMP;
    ip->ip_sum = 0;

    /* ICMP structure, check ip_icmp.h */
    icmp->icmp_type = type;
    icmp->icmp_code = 0;
    icmp->icmp_id = 123;
    icmp->icmp_seq = 0;
}

int send_packet(int sock_fd, struct icmp *icmp, struct ip *ip, char *send_buf, const char *data, int data_len) {
    int bytes_to_send = MAGIC_LEN + data_len + ICMP_DATA_OFFSET;
    struct sockaddr_in dst;
    dst.sin_family = AF_INET;

    /* Copy data to icmp data field */
    memcpy(send_buf + ICMP_DATA_OFFSET, MAGIC, MAGIC_LEN);
    memcpy(send_buf + ICMP_DATA_OFFSET + MAGIC_LEN, data, data_len);
    ip->ip_len = htons(bytes_to_send);

    /* Header checksums */
    icmp->icmp_cksum = 0;
    ip->ip_sum = cksum((unsigned short *)send_buf, ip->ip_hl);
    icmp->icmp_cksum = cksum((unsigned short *)icmp, sizeof(icmp) + MAGIC_LEN + data_len);
    //icmp->icmp_cksum = cksum((unsigned short *)icmp, sizeof(send_buf) - sizeof(struct icmp));

    /* Send packet */
    if (bytes_to_send != sendto(sock_fd, send_buf, bytes_to_send, 0, (struct sockaddr *)&dst, sizeof(dst))) {
        perror("send() error");
    }
}

int is_icmp_tunnel(const char* packet) {
    struct ip *ip;
    // memcpy(ip, packet, sizeof(struct ip));
    ip = (struct ip*)packet;
    if (ip->ip_p == IPPROTO_ICMP) {
        printf("%x\n", packet[sizeof(struct icmp)]);
    }
    return (ip->ip_p == IPPROTO_ICMP) && (0 == memcmp(packet + sizeof(struct icmp), MAGIC, MAGIC_LEN));
}

/* One's Complement checksum algorithm */
unsigned short cksum(unsigned short *addr, size_t len)
{
    int nleft = len;
    int sum = 0;
    unsigned short *w = addr;
    unsigned short answer = 0;

    while (nleft > 1) {
      sum += *(w++);
      nleft -= 2;
    }

    if (nleft == 1) {
      *(unsigned char *)(&answer) = *(unsigned char *)w;
      sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;

    return (answer);
}
