#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>

#define PORT 8081
#define MAX_EVENTS 100
#define BUFFER_SIZE 2048
#define MAX_CONNECTIONS 10

void handle_error(const char *error_out) 
{
    printf(error_out);
    exit(EXIT_FAILURE);
}

void cleanup(int server_fd, int epfd) 
{
    if (server_fd != -1) 
    {
        close(server_fd);
    }
    if (epfd != -1) 
    {
        close(epfd);
    }
}

void handle_client(int client_fd) 
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) 
    {
        printf("Read command failed");
        close(client_fd);
    }
    else if (bytes_read == 0) 
    {
        printf("Closing connection for file descriptor %d\n", client_fd);
        close(client_fd);
    } 
    else 
    {
        buffer[bytes_read] = '\0';
        printf("Received message: %s\n", buffer);
        const char *response = "Hello from the server\n";
        if (send(client_fd, response, strlen(response), 0) == -1) 
        {
            printf("Send  command failed");
            close(client_fd);
        }
    }
}


int main() 
{

    struct sockaddr_in server_addr;
    struct epoll_event event, events[MAX_EVENTS];
    int epfd = -1;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0) ;
    //printf("%d",server_fd) ;
    if (server_fd == -1) 
    {
        handle_error("Socket creation failed");
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    int bind_output =  bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ;
    //printf("%d",bind_output) ;
    if (bind_output == -1) 
    {
        cleanup(server_fd, epfd);
        handle_error("Bind operation failed cleanup completed");
    }
    int listen_output =  listen(server_fd, MAX_CONNECTIONS) ;
    //  printf("%d",listen_output) ;
    if (listen_output == -1) 
    {
        cleanup(server_fd, epfd);
        handle_error("Listen command failed  , cleanup completed");
    }

    epfd = epoll_create1(0) ;
    if (epfd == -1) 
    {
        cleanup(server_fd, epfd);
        handle_error("epoll_create1   function call failed");
    }

    event.events = EPOLLIN; 
    event.data.fd = server_fd; 
    int event_status = epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &event) ;
    //  printf("%d",event_status) ;
    //  printf("%d",event.data.fd) ;
    if (event_status == -1) 
    {
        cleanup(server_fd, epfd);
        handle_error("epoll_ctl failed for server_fd");
    }
    else 
    {
        printf("The socket event is registered wihtout any issue ") ;
        printf("\n") ;
        printf("Server listening on port %d\n", PORT);
    }

    while (1) 
    {
        int num_fds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (num_fds == -1) 
        {
            cleanup(server_fd, epfd);
            handle_error("epoll_wait  fucntion failed , cleanup handled properly");
        }

        for (int i = 0; i < num_fds; i++) 
        {
            if (events[i].data.fd == server_fd) 
            {
                int client_fd = accept(server_fd, NULL, NULL);
                event.events = EPOLLIN; 
                event.data.fd = client_fd; 
                if (client_fd == -1)
                {
                    printf("Accept function failed");
                    continue;
                }
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &event) == -1) 
                {
                    printf("epoll_ctl failed for client_fd");
                    close(client_fd);
                    continue;
                }

                printf("Recived Newe on file descriptor number %d\n", client_fd);
            } 
            else 
            {
                handle_client(events[i].data.fd);
            }
        }
    }

    cleanup(server_fd, epfd);
    return 0;
}
