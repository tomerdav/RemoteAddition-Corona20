#ifndef __TUN_H__
#define __TUN_H__

/**
 * Open a tun device
 *
 * @ret tun's fd
 */
int tun_alloc(char *dev);

#endif // __TUN_H__
