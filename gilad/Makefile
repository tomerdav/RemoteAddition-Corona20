CC=gcc
CFLAGS=-Wall -Werror


client: client.c tun_interface.c packet_tools.c utils.c tun_interface.h packet_tools.h utils.h
	$(CC) $(CFLAGS) -o client_prog client.c tun_interface.c packet_tools.c utils.c

run_client: client
	sleep 1 && sudo ./setup_client.sh &
	sudo ./client_prog

clean:
	rm -rf *.o *_prog
	sudo ./undo*.sh