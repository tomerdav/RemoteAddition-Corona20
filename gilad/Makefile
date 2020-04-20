CC=gcc
CFLAGS=-Wall -Werror


client: client.c tun_interface.c packet_tools.c tun_interface.h packet_tools.h
	$(CC) $(CFLAGS) -o client_prog client.c tun_interface.c packet_tools.c