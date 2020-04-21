#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "tunnel.h"
#include "icmp_cover.h"
#include "defs.h"


int main() {
    fd_set read_fds = {0};
    int err = EXIT_SUCCESS;
    int num_read = 0;
    char recv_buffer[BUFF_SIZE + 4];
    char* new_buffer = NULL;
    char* buffer = NULL;
    int new_size = 0;

    int fd = tun_alloc(TUN_NAME);
    if (fd == -1) {
        return -1;
    }

    printf("%s", "Interface tun33 created successfully\n");

    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(fd, &read_fds);

        err = select(fd + 1, &read_fds, NULL, NULL, NULL);
        
        if (err <= 0) {
            break;
        }
        
        num_read = tun_read(fd, recv_buffer, BUFF_SIZE + 4) - 4;
        buffer = recv_buffer + 4;
    
        if (num_read == -1) {
            break;
        }

        uint32_t dest = 0;
        new_size = remove_icmp_cover(buffer, &new_buffer, num_read, &dest);  

        if (new_size == -1) {
            break;
        }


        if (new_size < num_read) {
            err = send_icmp_uncovered_packet(new_buffer, new_size, dest); 
        }

        if (err == -1) {
            break;
        }

        if (new_size < num_read) {
            printf("uncovered packet sent\n");
        } else {
            printf("normal packet received\n");
            continue;
        }

        free(new_buffer);     
    }

 
    tun_close(fd);
    printf("%s", "Interface tun33 closed\n");

    return EXIT_SUCCESS;
}
