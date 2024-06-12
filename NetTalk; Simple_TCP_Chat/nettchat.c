#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>

#define BUFFER_SIZE 4096

void server(int portNum) {
    // If port number is 0, pick a random ephemeral-range port
    if (portNum == 0) {
        srandom(getpid());
        portNum = 0xc000 | (random() & 0x3fff);
    }

    printf("Listening on port %d\n", portNum);

    struct sockaddr_in ipOfServer;
    memset(&ipOfServer, 0, sizeof(struct sockaddr_in));

    ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_addr.s_addr = htonl(INADDR_ANY);
    ipOfServer.sin_port = htons(portNum);

    int listen_in = socket(AF_INET, SOCK_STREAM, 0);
    bind(listen_in, (struct sockaddr*)&ipOfServer, sizeof(ipOfServer));
    listen(listen_in, 20);

    int connection = accept(listen_in, (struct sockaddr*)NULL, NULL);
    close(listen_in);

    struct pollfd var[2];
    var[0].fd = 0;  // Standard input
    var[1].fd = connection;  // Connection socket

    var[0].events = POLLIN;
    var[1].events = POLLIN;

    while (1) { // Infinite loop
        int value = poll(var, 2, 60000); // 1-minute timeout
        char buffer[BUFFER_SIZE];

        if (value > 0) {
            if (var[0].revents & POLLIN) {
                int read_in = read(0, buffer, BUFFER_SIZE);
                write(connection, buffer, read_in);
            }
            if (var[1].revents & POLLIN) {
                int read_in = read(connection, buffer, BUFFER_SIZE);
                write(1, buffer, read_in);
                if (read_in == 0) {  // Connection closed
                    break;
                }
            }
        }
    }
    close(connection);
}

void client(char *ip_address, int port) {
    struct sockaddr_in ipOfServer;
    ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_addr.s_addr = inet_addr(ip_address);
    ipOfServer.sin_port = htons(port);

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    connect(clientSocket, (const struct sockaddr *)&ipOfServer, sizeof(ipOfServer));

    struct pollfd var[2];
    var[0].fd = 0;  // Standard input
    var[1].fd = clientSocket;  // Client socket
    var[0].events = POLLIN;
    var[1].events = POLLIN;

    while (1) {
        int value = poll(var, 2, 60000); // 1-minute timeout
        char buffer[BUFFER_SIZE];

        if (value > 0) {
            if (var[0].revents & POLLIN) {
                int read_in = read(0, buffer, BUFFER_SIZE);
                write(clientSocket, buffer, read_in);
            }
            if (var[1].revents & POLLIN) {
                int read_in = read(clientSocket, buffer, BUFFER_SIZE);
                write(1, buffer, read_in);
                if (read_in == 0) {  // Connection closed
                    break;
                }
            }
        }
    }
    close(clientSocket);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        server(0);  // No port specified, use random
    } else if (argc == 2) {
        int port = atoi(argv[1]);
        server(port);
    } else if (argc == 3) {
        char *ip_address = argv[1];
        int port = atoi(argv[2]);
        client(ip_address, port);
    } else {
        fprintf(stderr, "Usage: %s [server port] or %s [server IP] [server port]\n", argv[0], argv[0]);
        return 1;
    }
    return 0;
}
