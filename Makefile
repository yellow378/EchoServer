cc = $(which gcc)

hello: hello.c 
	cc hello.c -o hello

run: hello
	./hello

tcp: tcp.c tcp.h arg_parser.c arg_parser.h
	cc tcp.c arg_parser.c -o tcp

arg_parser: arg_parser.c
	cc -DSTANDALONE_TEST arg_parser.c -o arg_parser

clean: 
	rm -rf *.o hello tcp arg_parser

