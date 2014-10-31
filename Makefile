all:
	clear
	mkdir -p build/
	gcc src/erp_udp.c -o build/erp_udp -lpthread -lrt
	gcc src/udp_server.c -o build/udp_server
	gcc src/udp_client.c -o build/udp_client
server:
	build/udp_server 9001
client:
	build/udp_client 127.0.0.1 9000
server2:
	build/udp_server 9003
client2:
	build/udp_client 127.0.0.1 9002
run:
	build/erp_udp 500 5 5 9000 9001
clean:
	rm -rf build/
astyle:
	astyle --recursive --style=gnu --indent=tab *.c
