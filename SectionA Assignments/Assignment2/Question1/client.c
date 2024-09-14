#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 2048

int main() {
    int sock;
    struct sockaddr_in server_addr;
    const char *message = "Hello from client file ";
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket Failed ");
        exit(EXIT_FAILURE);
    }

    // sin port  for the port number used by the user . 
    // sin_family is for the struct of the IPV4 and deifnes the socket and realted .
    // AF_Inet is used for the ipv 4 and AF_Inet_6 is used fot eh IP v6
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    
    // inet_pton is use for the conversion of the IP from the textual format to the Bin format 
    // initally the Ip is in 127.0.0.1 string fromat after using this we can get this as a bin format 
    int binary_format_Ip  = inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) ;
    //printf("Bin format is " , binary_format_Ip) ;
    if (binary_format_Ip <= 0) {
        printf("The IP address is invalid") ;
        close(sock);
        exit(EXIT_FAILURE);
    }

    // connect() use to connet the Cleint and the server  ; returns a  integer value 
    int connection_status = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) ;
    if (connection_status < 0) {
        printf("Connection failed , there could be seevral issue ");
        printf("\n") ;
        printf("Server might not be initaited ") ;
        printf("\n") ;
        printf("Socket descriptor might be failed ") ;
        printf("\n") ;
        close(sock);
        exit(EXIT_FAILURE);
    }

    // send() is use to check the number of the bytes send after the connect() is estabilshed 
    ssize_t bytes_sent = send(sock, message, strlen(message), 0);
    if (bytes_sent < 0) 
    {
        printf("Zero byte are send , there is a issue ");
        close(sock);
        exit(EXIT_FAILURE);
    }
    else 
    {
    printf("Message sent to server\n");
    }

    // read is use to get the output or the messge from the server.c 
    // if the read is zero then there is a error 
    ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        printf("Read failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // this part is handling the null character from the bytes recived in the buffer from server.c
    buffer[bytes_read] = '\0';
    printf("Server response: %s\n", buffer);

    close(sock);
    return 0;
}
