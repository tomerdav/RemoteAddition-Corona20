#include <stdio.h>
#include <unistd.h>
#include <linux/if.h>

#include "tun.h"

#define TUN_ALLOC (1)
#define TUN_CLOSE (2)
#define TUN_READ (3)
#define TUN_WRITE (4)

#define OPT_EXIT (5)

int main() {
    int tun_fd = -1;
    int option = -1;
    int nread = -1;

    char tun_name[IFNAMSIZ] = "tun0";
    char buffer[1500];

    printf("%d - alloc\n%d - close\n\n%d - read\n%d - write\n\n%d - exit\n",
            TUN_ALLOC, TUN_CLOSE, TUN_READ, TUN_WRITE, OPT_EXIT);

    while(option != OPT_EXIT) {
        printf("Enter option: ");
        scanf("%d", &option);

        switch(option) {
        case TUN_ALLOC:
            tun_fd = tun_alloc(tun_name);
            if(tun_fd != -1) {
                printf("Create tunnel %s\n", tun_name);
            }
            break;
        case TUN_CLOSE:
            if(tun_fd != -1) {
                close(tun_fd);
                tun_fd = -1;
                printf("closed tunnel\n");
            } else {
                printf("Must open before!\n");
            }
            break;
        case TUN_READ:
            while(1) {
                nread = read(tun_fd, buffer, sizeof(buffer));
                if(nread < 0) {
                    printf("reading\n");
                    //close(tun_fd);
                }

                printf("Read %d bytes: %s\n", nread, buffer);
            }
            break;
        case TUN_WRITE:
            printf("Unimplemented error\n");
            break;
        case OPT_EXIT:
            printf("bye!\n");
            break;
        default:
            printf("Unknown option!\n");
            break;
        }
    }

    return 0;
}
