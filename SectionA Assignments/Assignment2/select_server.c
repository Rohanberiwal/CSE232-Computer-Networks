#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h> // Include this header for errno and EINTR

#define PORT 8081
#define BUFFER_SIZE 2048
#define MAX_CLIENTS 10

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    
    if (bytes_read <= 0) {
        if (bytes_read == 0) {
            printf("Client disconnected.\n");
        } else {
            perror("Read error");
        }
        close(client_socket);
        return;
    }
    
    buffer[bytes_read] = '\0';
    printf("Received message: %s\n", buffer);
    
    const char *response = "I have sent you the top two CPU-consuming processes.\n";
    send(client_socket, response, strlen(response), 0);
}

int main() {
    int server_fd, client_socket, max_sd;
    struct sockaddr_in address;
    fd_set readfds;

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Setup address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    int client_sockets[MAX_CLIENTS] = {0};

    while (1) {
        // Initialize the file descriptor set
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // Add client sockets to the set
        for (int i = 0; i < MAX_CLIENTS; i++) {
            client_socket = client_sockets[i];
            if (client_socket > 0) {
                FD_SET(client_socket, &readfds);
            }
            if (client_socket > max_sd) {
                max_sd = client_socket;
            }
        }

        // Wait for an activity on one of the sockets
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        
        if ((activity < 0) && (errno != EINTR)) {
            perror("Select error");
        }

        // Check for new connections
        if (FD_ISSET(server_fd, &readfds)) {
            socklen_t addrlen = sizeof(address);
            if ((client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
                perror("Accept failed");
                exit(EXIT_FAILURE);
            }
            printf("New connection: socket fd is %d\n", client_socket);

            // Add new socket to the array of sockets
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = client_socket;
                    break;
                }
            }
        }

        // Handle I/O for each client socket
        for (int i = 0; i < MAX_CLIENTS; i++) {
            client_socket = client_sockets[i];
            if (FD_ISSET(client_socket, &readfds)) {
                handle_client(client_socket);
                client_sockets[i] = 0; // Clear socket if it is closed
            }
        }
    }

    close(server_fd);
    return 0;
}
