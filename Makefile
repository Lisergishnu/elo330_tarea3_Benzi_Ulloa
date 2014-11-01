all:
	clear
	mkdir -p build/
	gcc src/erp_udp.c -o build/erp_udp -lpthread -lrt
	gcc src/udp_peer.c -o build/udp_peer -lpthread -lrt
peerA:
	build/udp_peer 127.0.0.1 9000 9003
peerB:
	build/udp_peer 127.0.0.1 9002 9001
run:
	build/erp_udp 500 5 5 9000 9001
clean:
	rm -rf build/
astyle:
	astyle --recursive --style=gnu --indent=tab *.c
