#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8081
#define BUFFER_SIZE 2048

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_connections>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_connections = atoi(argv[1]);
    if (num_connections <= 0) {
        fprintf(stderr, "Please enter a positive integer for connections.\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < num_connections; i++) {
        int sock;
        struct sockaddr_in server_addr;
        char message[] = "Please give me the top 2 processes of CPU consumption";
        char buffer[BUFFER_SIZE] = {0};

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("Socket creation failed");
            return -1;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
            perror("Invalid IP address");
            close(sock);
            return -1;
        }

        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Connection failed");
            close(sock);
            return -1;
        }

        send(sock, message, strlen(message), 0);
        printf("Message sent to server: %s\n", message);

        ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Response from server: %s\n", buffer);
        } else {
            perror("Read failed");
        }

        close(sock);
    }

    return 0;
}
