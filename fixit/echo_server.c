#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>

void handle_request(int client_fd) {
    int continue_reading = 1;
    while (continue_reading) {
        size_t max_size = 32;
        void *buf = malloc(max_size);
        size_t size = read(client_fd, buf, max_size);
        write(client_fd, buf, size);
        if (!size) {
            continue_reading = 0;
        }
    }
}

int main(int argc, char **argv) {
    int server_port = 62162;
    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    int socket_option = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &socket_option, sizeof(socket_option))) {
        fprintf(stderr, "Failed to accept client connection\n");
        exit(errno);
    }


    struct sockaddr_in server_address, client_address; 
    size_t client_address_length = sizeof(client_address);
    memset(&server_address, 0, sizeof(server_address));
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(server_port);

    if (bind(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        fprintf(stderr, "Failed to bind to address %i \n", errno);
        exit(errno);
    }

    if (listen(socket_fd, 1024) == -1) {
        fprintf(stderr, "Failed to begin listening for tcp connection\n");
        exit(errno);
    }

    while(1) {
        int client_fd = accept(socket_fd, (struct sockaddr*) &client_address, (socklen_t *) &client_address_length);
        if (client_fd < 0) {
            fprintf(stderr, "Failed to accept client connection\n");
            continue;
        }
        printf("Connected to client\n");
        handle_request(client_fd);
    }
    return 0;
}
