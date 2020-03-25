#ifndef TUNNEL_H_DEFINE
#define TUNNEL_H_DEFINE

int tun_alloc(char* dev_name);
void tun_close(int fd);
void tun_write();
void tun_read();


#endif
