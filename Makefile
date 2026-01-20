cc = $(which gcc)

hello: hello.c 
	cc hello.c -o hello

run: hello
	./hello

tcp: tcp.c tcp.h arg_parser.c arg_parser.h floating_point.h floating_point.c
	cc tcp.c arg_parser.c  floating_point.c -o tcp

arg_parser: arg_parser.c
	cc -DSTANDALONE_TEST arg_parser.c -o arg_parser

floating_point: floating_point.c 
	cc -DSTANDALONE_TEST floating_point.c -o floating_point

clean: 
	rm -rf *.o hello tcp arg_parser

