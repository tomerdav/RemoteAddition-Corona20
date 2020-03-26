/*
 * allocation of tunnel
 */
int tun_alloc();

void tun_close(int tun_fd);

int tun_write(int tun_fd, const char *buf, int size);

int tun_read(int tun_fd, char *buf, int size);

