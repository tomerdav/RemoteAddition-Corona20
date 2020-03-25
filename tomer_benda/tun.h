#ifndef __TUN_H__
#define __TUN_H__

/**
 * Open a tun device
 *
 * @ret tun's fd
 */
int tun_alloc(char *dev);

/**
 * Close a given tunnel by closing it's file descriptor
 */
void tun_close(int fd);

#endif // __TUN_H__
