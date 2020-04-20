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
    char buffer[BUFF_SIZE];
    char* new_buffer = NULL;
    int new_size = 0;

    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (fd == -1) {
        return -1;
    }

    printf("%s", "Sniff started\n");

    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(fd, &read_fds);

        err = select(fd + 1, &read_fds, NULL, NULL, NULL);
        
        if (err <= 0) {
            break;
        }
        
        num_read = socket_read(fd, buffer, BUFF_SIZE);

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
        }

        if (err == -1) {
            break;
        }

        free(new_buffer);     
    }

 
    close(fd);
    printf("%s", "Sniff stopped\n");

    return EXIT_SUCCESS;
}
