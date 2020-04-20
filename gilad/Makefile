CC=gcc
CFLAGS=-Wall -Werror


client: client.c tun_interface.c packet_tools.c utils.c tun_interface.h packet_tools.h utils.h
	$(CC) $(CFLAGS) -o client_prog client.c tun_interface.c packet_tools.c utils.c

run_client: client
	sleep 1 && sudo ./setup_client.sh &
	sudo ./client_prog

server: server.c tun_interface.c packet_tools.c utils.c tun_interface.h packet_tools.h utils.h
	$(CC) $(CFLAGS) -o server_prog server.c tun_interface.c packet_tools.c utils.c

run_server: server
	sleep 1 && sudo ./setup_server.sh &
	sudo ./server_prog

clean:
	rm -rf *.o *_prog
	sudo ./undo_client.sh
	sudo ./undo_server.sh