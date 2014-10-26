all:
	clear
	mkdir -p build/
	gcc src/udp_server.c -o build/udp_server
	gcc src/udp_client.c -o build/udp_client
server:
	build/udp_server
client:
	build/udp_client 127.0.0.1
clean:
	rm -rf build/
