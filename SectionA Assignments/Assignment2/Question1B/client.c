
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8081
#define BUFFER_SIZE 2048

void *client_task(void *arg) {
    int sock;
    struct sockaddr_in server_addr;
    const char *message = "Hello from client";
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return NULL;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IP address
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        printf("Invalid address");
        close(sock);
        return NULL;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connection failed");
        close(sock);
        return NULL;
    }

    // Send message
    ssize_t bytes_sent = send(sock, message, strlen(message), 0);
    if (bytes_sent < 0) {
        printf("Send failed");
        close(sock);
        return NULL;
    }

    // Read response
    ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        printf("Read failed");
        close(sock);
        return NULL;
    } else if (bytes_read == 0) {
        printf("Server disconnected unexpectedly\n");
        close(sock);
        return NULL;
    }

    buffer[bytes_read] = '\0';  // Null-terminate the buffer
    printf("Server response: %s\n", buffer);

    close(sock);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Create threads
    pthread_create(&t1, NULL, client_task, NULL);
    pthread_create(&t2, NULL, client_task, NULL);

    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

