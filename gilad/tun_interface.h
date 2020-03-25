/**
 * Includes functions to create and use a tun virtual interface.
 */

#include <unistd.h>

/**
 * Creates the tun interface
 * 
 * @param device_name   [IN, OUT]   The name of the device (for example "tun0", "tun1", etc.).
 *                                  length(device_name) <= IFNAMESZ should hold (IFNAMESZ is defined in linux/if.h)
 * 
 * @return On success returns the new device's fd, on failure returns a negative value.
 */
int tun_alloc(char* device_name);


/**
 * Closes the given tun device.
 * 
 * @param fd   [IN]    The file descriptor of the device.
 * 
 * @return On success returns 0
 */
int tun_close(int fd);


/**
 * Writes to a given tun device.
 * 
 * @param fd        [IN]    The file descriptor of the device.
 * @param buffer    [IN]    A buffer with the data to write.
 * @param count     [IN]    The amount of bytes to write.
 * 
 * @return On success, the amount of bytes written successfuly to the tun. On failure -1.
 */
ssize_t tun_write(int fd, const void* buffer, size_t count);


/**
 * Reads to a given tun device.
 *
 * @param fd        [IN]    The file descriptor of the device.
 * @param buffer    [OUT]   A buffer to put the read data in.
 * @param count     [IN]    The amount of bytes to read.
 * 
 * @return On success, the amount of bytes read successfuly from the tun. On failure -1.
 */
ssize_t tun_read(int fd, void* buffer, size_t count);