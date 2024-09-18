#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8081
#define BUFFER_SIZE 2048

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    const char *response = "Hello server  file " ;

    // use to read the buffer byptes reciveed we remove 1 because of the null character
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        printf("Read operation failed");
        pthread_exit(NULL);
    }
    // same as the client command remove the bufffer's null character 
    buffer[bytes_read] = '\0';
    printf("Message received: %s\n", buffer);

    // here the client send the constant character response ie hello from the client file
    ssize_t bytes_sent = send(client_socket, response, strlen(response), 0);
    if (bytes_sent < 0) {
        printf("Send failed");
        pthread_exit(NULL);
    }

    printf("Response sucessfully sent to client \n");

    close(client_socket);
}

int main() 
{
    // insttantiae the server file descriptor to listne the ports 

    int server_fd ;
    //  the socket_in  has the server_address and teh port number .
    struct sockaddr_in server_addr ;
    struct sockaddr_in client_addr;
    int addrlen = sizeof(server_addr);
    socklen_t client_len = sizeof(client_addr);

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
        close(server_fd) ;
        exit(EXIT_FAILURE);
    }
    /*
    else 
    {
        printf("Bind sucessful") ;
    }
    */
    int max_queue_user = 10 ;
    // Listen to the cleint after the bind is done 
    if (listen(server_fd, max_queue_user) < 0) {
        printf("Server listen command failed ");
        close(server_fd) ;
        exit(EXIT_FAILURE);
    }
    /*
    else 
    {
        printf("Listen Command is sucessful") ;
    }
    */

    printf("Server listening on port %d\n", PORT);

    // Use a while loop where we accpe the message from the cleint scoket to the server side .
    //  this is done after the establishment of teh TCP from the listena and bind()
    while (1) {
        // This is use to accpet the message from the client 
        int client_socket;
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            printf("Accept operation failed");
            close(client_socket) ;
            exit(EXIT_FAILURE);
        }
        /*
        else 
        {
            handle_client(client_socket) ;
        }
        */

    }

    close(server_fd);
    return 0;
}
