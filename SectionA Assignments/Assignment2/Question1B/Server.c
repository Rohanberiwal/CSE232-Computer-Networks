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

void *handle_client(void *client_socket) {
    int sock = *(int *)client_socket;
    char buffer[BUFFER_SIZE] = {0};
    const char *response = "Hello from server file";

    // PID to print the thread Id that is runnign as this is a code for multi threading 

    pid_t tid = syscall(SYS_gettid);
    count_user = count_user + 1;
    printf("------Thread  %d statistics  ------", count_user) ;
    printf("\n") ;
    printf("Thread PID(process id on the CPU): %d, Thread ID: %ld\n", getpid(), tid);
    printf("\n") ;
    // Read the buffer bytes received (remove 1 because of the null character)
    ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        printf("Read operation failed\n");
        close(sock);
        pthread_exit(NULL);
    }

    // Null-terminate the buffer
    buffer[bytes_read] = '\0';
    printf("Message received: %s\n", buffer);

    // Send response to the client
    ssize_t bytes_sent = send(sock, response, strlen(response), 0);
    if (bytes_sent < 0) {
        printf("Send failed\n");
        close(sock);
        pthread_exit(NULL);
    }

    printf("Response successfully sent to client\n");

    close(sock);
    pthread_exit(NULL);
}


int main() 
{
    // insttantiae the server file descriptor to listne the ports 
    // make  the client socket that is  use to recive the messager from the client

    int server_fd, client_socket;
    //  the socket_in  has the server_address and teh port number .
    struct sockaddr_in server_addr, client_addr;

    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // There is a additional sin_add.s_addr this is use to specify which socket must  the server bidn to . 
    // The rest of the add are the smae  , teh family is the IPV4 and the sin_port is the port no
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    

    // do the bind operation 
    // bind does the binding to teh port mentioned here is
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Bind operation  failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    int max_queue_user = 10 ;
    // Listen to the cleint after the bind is done 
    if (listen(server_fd, max_queue_user) < 0) {
        printf("Server listen command failed ");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Use a while loop where we accpe the message from the cleint scoket to the server side .
    //  this is done after the establishment of teh TCP from the listena and bind()
    while (1) {
        // This is use to accpet the message from the client 
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            printf("Accept operation failed");
            continue;
        }

        // this is the concurrent function thta is handling  the cliennt handle operation 
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket) != 0) 
        {
            printf("Thread creation failed");
            close(client_socket);
        } 

        else 
        {
            pthread_detach(thread_id);
        }
    }

    close(server_fd);
    return 0;
}
