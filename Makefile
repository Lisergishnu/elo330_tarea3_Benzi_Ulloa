all:
	clear
	mkdir -p build/
	gcc src/erp_udp.c -o build/erp_udp -lpthread 
	gcc src/udp_server.c -o build/udp_server
	gcc src/udp_client.c -o build/udp_client
server:
	build/udp_server
client:
	build/udp_client 127.0.0.1
run:
	build/erp_udp 500 5 5 9000 9001
clean:
	rm -rf build/
