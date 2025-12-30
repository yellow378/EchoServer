#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

/*
 * bind
 * listen
 * accept
 */

void create_server() {
    int my_socket = socket(AF_INET, SOCK_STREAM, 0);
    printf("socket: %d\n", my_socket);

    struct in_addr sin_addr;
    sin_addr.s_addr = INADDR_ANY;

    struct sockaddr_in my_addr;
    my_addr.sin_addr = sin_addr;
    my_addr.sin_port = htons(6789);
    my_addr.sin_family = AF_INET;

    if (bind(my_socket, (const struct sockaddr *)&my_addr,
             sizeof(struct sockaddr_in)) == -1) {
        printf("bind failed!\n");
    } else {
        printf("bind successful!\n");
    }

    if (listen(my_socket, 0) == -1) {
        printf("listen failed!\n");
    } else {
        printf("listen successful!\n");
    }

    int acc_socket = accept(my_socket, NULL, NULL);
    char buffer[100] = "Hello\n";
    write(acc_socket, buffer, 7);
}

int main() {
    create_server();
    return 0;
}
