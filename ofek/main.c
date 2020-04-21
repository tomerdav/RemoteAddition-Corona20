#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "tunnel.h"
#include "icmp_cover.h"
#include "utils.h"
#include "defs.h"

int main() {
    fd_set read_fds = {0};
    int err = EXIT_SUCCESS;
    int num_read = 0;
    char recv_buffer[RECV_BUFF_SIZE];
    char* new_buffer = NULL;
    char* buffer = recv_buffer + IGNORE_OFFSET;
    int new_size = 0;

    int fd = tun_alloc(TUN_NAME);

    if (fd == ERROR_CODE) {
        return EXIT_FAILURE;
    }

    printf("%s", "Interface tun33 created successfully\n");

    while (true) {
        num_read = wait_for_tun_reading(fd, recv_buffer, RECV_BUFF_SIZE) - IGNORE_OFFSET;

        if (num_read == ERROR_CODE) {
            break;
        }

        new_size = build_icmp_cover(buffer, &new_buffer, num_read, false);

        if (new_size == ERROR_CODE) {
            break;
        }

        err = send_icmp_covered_packet(new_buffer, new_size, SERVER_ADDR);
        free(new_buffer);

        if (err == ERROR_CODE) {
            break;
        }

        printf("packet sent\n");     
    }
 
    tun_close(fd);
    printf("%s", "Interface tun33 closed\n");

    return EXIT_SUCCESS;
}

