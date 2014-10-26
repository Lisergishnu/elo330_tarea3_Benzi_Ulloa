all:
	mkdir -p build/
	gcc src/main.c -o build/erp_udp
run:
	build/erp_upd 100 5 5 9000 9001
clean:
	rm -rf build/
