#ifndef TUNNEL_H_DEFINE
#define TUNNEL_H_DEFINE

int tun_alloc(char* dev_name);
void tun_close(int fd);
int tun_write(int fd, char* buffer, size_t size);
int tun_read(int fd, char* buffer, size_t size);


#endif

