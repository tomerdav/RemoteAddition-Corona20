#pragma once

char* DEV_NAME = "tun0";
int BUF_SIZE = 1500;

int tun_alloc(char *dev);

int tun_close(int fd);

int tun_write(int fd, const void* buffer);

int tun_read(int fd, void* buffer);
