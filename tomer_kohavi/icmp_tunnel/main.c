// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <netinet/in.h>

// #include <netinet/in_systm.h>
// #include <netinet/ip.h>
// #include <netinet/ip_icmp.h>
// #include <string.h>
// #include <arpa/inet.h>
// #include <sys/select.h>
// #include <sys/time.h>

#include "icmp_tun.c"
#include "icmp_packet.c"

const char* SERVER_IP = "192.168.0.244";

int main() {
    int tun_fd;
    char buffer[BUF_SIZE];
    int bytes_read;

    int sock_fd;
    char send_buf[1500];
    struct ip *ip = (struct ip *)send_buf;
    struct icmp *icmp = (struct icmp *)(ip + 1);

    tun_fd = tun_alloc(DEV_NAME);
    
    if (-1 == tun_fd) {
        perror("tun_alloc()");
        return -1;
    }

    set_src_ip(ip);
    set_dst_ip(ip, SERVER_IP);

    sock_fd = create_socket();

    create_icmp_packet(icmp, ip, send_buf);

    while(1) {

        if (-1 == (bytes_read = tun_read(tun_fd, (void *)buffer))) {
            close(sock_fd);
            tun_close(tun_fd);
            return -1;
        }
        send_packet(sock_fd, icmp, ip, send_buf, buffer, bytes_read);
    }

    tun_close(tun_fd);
    close(sock_fd);

    return 0;
}
