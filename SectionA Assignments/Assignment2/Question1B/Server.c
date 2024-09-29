#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h> 
#include <bits/syscall.h>

#define PORT 8081
#define BUFFER_SIZE 2048


int count_user  = 0 ;

void *handle_client(void *arg) {
    int new_socket = *(int *)arg;
    free(arg); // Free the allocated memory for the socket
    char buffer[1024] = {0};
    const char *hello = "Hello from server";

    pid_t tid = syscall(SYS_gettid);
    count_user = count_user + 1;
    printf("------Thread  %d statistics  ------", count_user) ;
    printf("\n") ;
    printf("Thread PID(process id on the CPU): %d, Thread ID: %ld\n", getpid(), tid);
    printf("\n") ;

    ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        perror("Read error");
        close(new_socket);
        return NULL;
    } else if (bytes_read == 0) {
        printf("Client disconnected unexpectedly\n");
        close(new_socket);
        return NULL;
    }
    
    printf("Message received: %s\n", buffer);

    ssize_t bytes_sent = send(new_socket, hello, strlen(hello), 0);
    if (bytes_sent < 0) {
        perror("Send error");
        close(new_socket);
        return NULL;
    }

    printf("Hello message sent\n");
    close(new_socket);
    return NULL;
}


int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        printf("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8081); // Changed to 8081

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        printf("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        int *new_socket = malloc(sizeof(int));
        if (new_socket == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }

        if ((*new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            printf("Accept failed");
            free(new_socket); // Free memory if accept fails
            continue;
        }

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, new_socket) != 0) {
            printf("Thread creation failed");
            close(*new_socket);
            free(new_socket);
        } else {
            pthread_detach(thread_id); // Detach the thread to clean up resources automatically
        }
    }

    close(server_fd);
    return 0;
}

