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
    //  basic sockket decleration 
    // idea taken from the tut slide and the github repo codes 
    int sock;
    struct sockaddr_in server_addr;
    char message[BUFFER_SIZE];
    char buffer[BUFFER_SIZE] = {0};
    // the thread id on the pc can be greater than 2^^31 ideall is have to smaller but in case a proper handle is beign exec() 
    long thread_id = (long)arg; 

    strcpy(message, "GET ME TOP TWO CPU PROCESS INFORMATION");
    printf("Thread %ld requesting top CPU processes.\n", thread_id);

    sock = socket(AF_INET, SOCK_STREAM, 0) ;
    if (sock < 0)
    {
        handle_error("Socket creation failed on the client");
    }

    // basci computaiton on the client side
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) 
    {
        close(sock);
        handle_error("Invalid IP address cleanup done");
    }
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        close(sock);
        handle_error("Connection failed cleanup done ");
    }

    ssize_t bytes_sent = send(sock, message, strlen(message), 0);
    if (bytes_sent < 0) 
    {
        close(sock);
        handle_error("Send failed cleanup done");
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
        // remove the null paramter at the end of te buffer as the number of therecived niits are not zerop but greater
        buffer[bytes_read] = '\0';
        // print that 
        printf("Server response: %s\n", buffer);
    }
    close(sock);
    return NULL;
}

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        fprintf(stderr, "error after %s connections are supposed to be specified", argv[0]);
        printf("\n") ;
        exit(EXIT_FAILURE);
    }
    int num_connections = atoi(argv[1]);
    // Check if the number of connections is a positive integer
    if (num_connections <= 0) 
    {
        fprintf(stderr, "Error please enter a positive N value.");
        printf("\n") ;
        exit(EXIT_FAILURE);
    }

    // dyanmci memmory allocation of as the number of thread can  be   larger than the abailit for the seerver to handle 
    // In that case the number of the extra cleitn will be connetct to the sever eadfter a certasin delay  
    // that extrra cleitn are supposed to wait in the queue 
    // in the server.c i have written the max connect limit as the global var ,  that can be alterated and the corrposoing chnage inthe biffer size are to be doen 
    pthread_t *threads = malloc(num_connections * sizeof(pthread_t));
    if (threads == NULL) 
    {
        handle_error("Memory allocation failed");
    }
    // reaqson for he long is simialr tio the above  ,  there could be a lot number of the threads 
    //  mkaing the chaneg in  the code of  the server to hanle more than 2^31 wouyld lead to a buffer iverfill
    //  long is use to handle that condition
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
        // join the output  to get the result that is the message from the client side 
        pthread_join(threads[i], NULL);
    }

    free(threads);
    return 0;
}
