#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8081
#define BUFFER_SIZE 2048

void handle_error(const char *error_out) 
{
    printf(error_out);
    exit(EXIT_FAILURE);
}

void *client_thread(void *arg) 
{
    int sock;
    struct sockaddr_in server_addr;
    char message[BUFFER_SIZE];
    char buffer[BUFFER_SIZE] = {0};
    long thread_id = (long)arg; 

    sprintf(message, "Hello from client thread %ld", thread_id);
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
    {
        handle_error("Socket creation failed on the client");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) 
    {
        close(sock);
        handle_error("Invalid IP address");
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        close(sock);
        handle_error("Connection failed");
    }

    ssize_t bytes_sent = send(sock, message, strlen(message), 0);
    if (bytes_sent < 0) 
    {
        close(sock);
        handle_error("Send failed");
    }
    else 
    {
        printf("Message sent to server: %s\n", message);
    }

    ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) 
    {
        close(sock);
        handle_error("Read failed");
    }
    else 
    {
        buffer[bytes_read] = '\0';
        printf("Server response: %s\n", buffer);
    }
    close(sock);
    return NULL;
}

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        printf("argmentation issue in the code ") ;
        exit(EXIT_FAILURE);
    }
    
    int num_connections = atoi(argv[1]);
    if (num_connections <= 0) 
    {
        fprintf(stderr, "Error: Please enter a positive integer for connections.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t *threads = malloc(num_connections * sizeof(pthread_t));
    if (threads == NULL) 
    {
        handle_error("Memory allocation failed");
    }

    for (long i = 0; i < num_connections; i++) 
    {
        if (pthread_create(&threads[i], NULL, client_thread, (void *)i) != 0) 
        {
            free(threads);
            handle_error("Thread creation failed");
        }
    }

    for (int i = 0; i < num_connections; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    return 0;
}
