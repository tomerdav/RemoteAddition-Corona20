#include <sys/types.h>

/**
 * Wait for file to be ready for reading.
 * 
 * @param fd        [IN]    The file's descriptor
 * @param timeout   [IN]    The maximal waiting period (in seconds)
 * 
 * @return Ready: ret > 0, Timeout: ret == 0, Error: ret < 0
 */
int wait_for_read(int fd, time_t timeout);