#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8081
#define BUFFER_SIZE 4096 

void *client_thread(void *arg) {
    int sock;
    struct sockaddr_in server_addr;
    const char *message = "Hello from client file";
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(sock);
        pthread_exit(NULL);
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        pthread_exit(NULL);
    }

    ssize_t bytes_sent = send(sock, message, strlen(message), 0);
    if (bytes_sent < 0) {
        perror("Send failed");
        close(sock);
        pthread_exit(NULL);
    }
    printf("Message sent to server\n");

    ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Read failed");
        close(sock);
        pthread_exit(NULL);
    }

    buffer[bytes_read] = '\0';
    printf("Server response: %s\n", buffer);

    close(sock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_connections>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_connections = atoi(argv[1]);
    if (num_connections <= 0) {
        fprintf(stderr, "Number of connections must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t *threads = malloc(num_connections * sizeof(pthread_t));
    if (threads == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_connections; i++) {
        if (pthread_create(&threads[i], NULL, client_thread, NULL) != 0) {
            perror("Thread creation failed");
            free(threads);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_connections; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    return 0;
}
