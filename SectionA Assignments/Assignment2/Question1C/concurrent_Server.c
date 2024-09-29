#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8081
#define BUFFER_SIZE 2048
#define MAX_CONNECTIONS 10

void handle_error(const char *error_out) 
{
    printf(error_out);
    exit(EXIT_FAILURE);
}

void handle_client(int client_fd) 
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) 
    {
        printf("Read command failed\n");
        close(client_fd);
        return;
    } 
    else if (bytes_read == 0) 
    {
        printf("Closing connection for file descriptor %d\n", client_fd);
        close(client_fd);
        return;
    } 
    else 
    {
        buffer[bytes_read] = '\0';
        printf("Received message: %s\n", buffer);
        const char *response = "Hello from the server\n";
        if (send(client_fd, response, strlen(response), 0) == -1) 
        {
            printf("Send command failed\n");
        }
    }
    close(client_fd);
}

int main() 
{
    struct sockaddr_in server_addr;
    int server_fd;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        handle_error("Socket creation failed");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) 
    {
        close(server_fd);
        handle_error("Bind operation failed");
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CONNECTIONS) == -1) 
    {
        close(server_fd);
        handle_error("Listen command failed");
    }

    printf("Server listening on port %d\n", PORT);

    while (1) 
    {
        // Accept a new client connection
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd == -1) 
        {
            printf("Accept function failed\n");
            continue;
        }

        printf("Received new connection on file descriptor %d\n", client_fd);

        // Create a new thread to handle the client
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, (void *(*)(void *))handle_client, (void *)(intptr_t)client_fd) != 0) 
        {
            perror("Failed to create thread");
            close(client_fd);
        }
        pthread_detach(thread_id); // Detach the thread to avoid memory leaks
    }

    close(server_fd);
    return 0;
}

