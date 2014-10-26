all:
	clear
	mkdir -p build/
	gcc src/main.c -o build/erp_udp
	gcc src/udp_server.c -o build/udp_server
	gcc src/udp_client.c -o build/udp_client
server:
	build/udp_server
client:
	build/udp_client 127.0.0.1
run:
	build/erp_upd 100 5 5 9000 9001
clean:
	rm -rf build/
