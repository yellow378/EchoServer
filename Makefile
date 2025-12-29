cc = $(which gcc)

hello: hello.c 
	cc hello.c -o hello

run: hello
	./hello

clean: 
	rm -rf *.o hello

