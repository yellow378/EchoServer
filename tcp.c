#include "tcp.h"
#include "arg_parser.h"
#include <sys/socket.h>

int create_server() {
    /*
     * socket will create a unbound socket
     * domain: AF_INET- Address Family Internet
     * type: SOCK_STREAM
     * protocol: 0
     */
    int my_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (my_socket == -1) {
        printf("Error: Create socket failed!\n");
        return -1;
    } else {
        printf("socket: %d\n", my_socket);
    }

    /*
     * create a socket Address
     * Note: It's import to covert host endian to net endian
     */
    struct sockaddr_in my_addr;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(6789);
    my_addr.sin_family = AF_INET;
    char addr_string[17];
    if (NULL == inet_ntop(AF_INET, &my_addr.sin_addr, addr_string,
                          sizeof(addr_string))) {
        printf("Error: Address convert failed!\n");
        return -1;
    } else {
        printf("address: %s, %d\n", addr_string, ntohs(my_addr.sin_port));
    }

    /*
     * Set socket to enable reuse
     */
    int reuse = 1;
    if (setsockopt(my_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
        0) {
        perror("set socket opt to address reuse failed");
        return -1;
    }

    /*
     * Bind address to Socket
     */
    if (bind(my_socket, (const struct sockaddr *)&my_addr,
             sizeof(struct sockaddr_in)) == -1) {
        perror("bind failed!");
    } else {
        printf("bind successful!\n");
    }

    /*
     * Make socket listen address and port
     */
    if (listen(my_socket, 0) == -1) {
        printf("listen failed!\n");
    } else {
        printf("listen successful!\n");
    }

    return my_socket;
}

int main() {
    int server = create_server();
    while (1) {
        /*
         * accept: while a connection connected, while create a
         * socket_pair<server_socket, client_socket>
         */
        int client = accept(server, NULL, NULL);
        int pid = fork();
        if (pid == 0) {
            /* Child Process */
            close(server);
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(struct sockaddr_in);
            getpeername(client, (struct sockaddr *)&client_addr, &client_len);
            char client_addr_string[17];
            if (NULL == inet_ntop(AF_INET, &client_addr.sin_addr,
                                  client_addr_string,
                                  sizeof(client_addr_string))) {
                perror("Address convert failed:");
                return 0;
            }
            int client_port = ntohs(client_addr.sin_port);
            printf("Connect from %s:%d\n", client_addr_string, client_port);

            char buffer[1000] = "Hello, welcom to my EchoServer\n";
            send(client, buffer, 1000, MSG_NOSIGNAL);
            ssize_t recv_size;
            while ((recv_size = recv(client, buffer, 999, MSG_NOSIGNAL)) > 0) {
                buffer[recv_size] = '\0';
                printf("Server Recv[%s:%d]: %s\n", client_addr_string,
                       client_port, buffer);
                size_t arg_count;
                char **args = arg_pars(buffer, &arg_count);
                if (args != NULL) {
                    for (size_t i = 0; i < arg_count; i++) {
                        snprintf(buffer, sizeof(buffer), "arg %zu : %s\n", i,
                                 args[i]);
                        send(client, buffer, strlen(buffer), MSG_NOSIGNAL);
                        free(args[i]);
                    }
                    free(args);
                }

                // send(client, buffer, recv_size, MSG_NOSIGNAL);
            }
            printf("Good bye!\n");
            return 0;
        } else {
            /* Parent Process
             * Nothing to do
             */
            // Parent child close client socket
            close(client);
            while (waitpid(-1, NULL, WNOHANG) > 0) {
            }
        }
    }
    return 0;
}
