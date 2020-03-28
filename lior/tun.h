#pragma once

/*
 * allocation of tunnel
 * @parm dev - the name of the device that will be created
 * @return - return the file descriptor of the tun device
 */
int tun_alloc(char *dev);

/*
 * close the tunnel
 * @parm tun_fd - the fd of the tunnel
 */
void tun_close(int tun_fd);

/*
 * write to tunnel 
 * @parm tun_fd - the tunnel file descriptor
 * @parm buf - the buf to write from
 * @parm size - size of bytes to write from buffer
 * @return - number of bytes written
 */
int tun_write(int tun_fd, const char *buf, int size);

/*
 * read from tunnel 
 * @parm tun_fd - the tunnel file descriptor
 * @parm buf - the buf to write to
 * @parm size - size of bytes to read from tunnel
 * @return - number of bytes read
 */
int tun_read(int tun_fd, char *buf, int size);

