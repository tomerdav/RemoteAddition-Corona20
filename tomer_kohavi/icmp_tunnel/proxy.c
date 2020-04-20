#include "icmp_tun.c"
#include "icmp_packet.c"

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

    sock_fd = create_socket();

    create_icmp_packet(icmp, ip, send_buf, ICMP_ECHOREPLY);

    while(1) {

        if (-1 == (bytes_read = tun_read(tun_fd, (void *)buffer))) {
            close(sock_fd);
            tun_close(tun_fd);
            return -1;
        }
        if (is_icmp_tunnel(buffer + 4)) {
            struct ip *recv_ip;
            recv_ip = (struct ip*)(buffer + 4);
            ip->ip_dst = recv_ip->ip_src;
            send_packet(sock_fd, icmp, ip, send_buf, buffer + 4, bytes_read - 4);
        }
    }

    tun_close(tun_fd);
    close(sock_fd);

    return 0;
}
