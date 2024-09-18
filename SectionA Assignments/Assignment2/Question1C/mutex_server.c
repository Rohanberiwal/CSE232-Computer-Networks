#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define PORT 8081
#define BUFFER_SIZE 4096 

int count_user = 0;
pthread_mutex_t count_mutex;

void *handle_client(void *client_socket) {
    int sock = *(int *)client_socket;
    free(client_socket);  // Free the allocated memory
    char buffer[BUFFER_SIZE] = {0};
    const char *response = "Hello from server file";

    pid_t tid = syscall(SYS_gettid);
    
    pthread_mutex_lock(&count_mutex);
    count_user++;
    printf("------Thread %d statistics ------\n", count_user);
    pthread_mutex_unlock(&count_mutex);

    ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Read failed");
        close(sock);
        pthread_exit(NULL);
    } else if (bytes_read == 0) {
        printf("Thread %lu: Client disconnected\n", pthread_self());
        close(sock);
        pthread_exit(NULL);
    }

    buffer[bytes_read] = '\0';
    printf("Message received: %s\n", buffer);

    ssize_t bytes_sent = send(sock, response, strlen(response), 0);
    if (bytes_sent < 0) {
        perror("Send failed");
        close(sock);
        pthread_exit(NULL);
    }

    printf("Response successfully sent to client\n");
    close(sock);
    pthread_exit(NULL);
}

int main() {
    int server_fd, *client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id;

    pthread_mutex_init(&count_mutex, NULL);  // Initialize mutex

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind operation failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    int max_queue_user = 10;
    if (listen(server_fd, max_queue_user) < 0) {
        perror("Server listen command failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        client_socket = malloc(sizeof(int));
        if (!client_socket) {
            perror("Failed to allocate memory");
            continue;
        }

        *client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (*client_socket < 0) {
            perror("Accept operation failed");
            free(client_socket);
            continue;
        }

        if (pthread_create(&thread_id, NULL, handle_client, client_socket) != 0) {
            perror("Thread creation failed");
            close(*client_socket);
            free(client_socket);
        } else {
            pthread_detach(thread_id);
        }
    }

    close(server_fd);
    pthread_mutex_destroy(&count_mutex);
    return 0;
}
