#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <dirent.h>

#define PORT 8081
#define MAX_EVENTS 100
#define MAX_CPU_PROCESSES 1024
#define BUFFER_SIZE 2048
#define MAX_CONNECTIONS 10


typedef struct info_list {
    int pid;
    char name[256];
    long total_cpu_time; 
    struct info_list* next;
} info_list;

info_list* create_node(int pid, const char *name, long total_cpu_time) {
    info_list *new_node = malloc(sizeof(info_list));
    if (!new_node) {
        printf("Malloc failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->pid = pid;
    strcpy(new_node->name, name);
    new_node->total_cpu_time = total_cpu_time; // Store total CPU time
    new_node->next = NULL;
    return new_node;
}

void add_node(info_list **head, int pid, const char *name, long total_cpu_time) {
    info_list *new_node = create_node(pid, name, total_cpu_time);
    new_node->next = *head; // Insert at the beginning
    *head = new_node;
}

int scan_process_file(int pid, info_list **process_info) 
{
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid); 
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Failed to open file\n");
        return -1;
    }

    int ignored;
    char name[256];
    long utime, stime;

    if (fscanf(file, "%d %s %*c %*d %*d %*d %*d %*d %*d %*d %*d %ld %ld", 
               &ignored, name, &utime, &stime) != 4) {
        fclose(file);
        printf("File read failed\n");
        return -1; 
    }

    fclose(file);
    long total_cpu_time = utime + stime; // Calculate total CPU time
    add_node(process_info, pid, name, total_cpu_time); // Add node with total CPU time
    return 0; 
}

void sort_process_array(info_list **process_array, int count) {
    // Simple Bubble Sort for demonstration
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (process_array[j] != NULL && process_array[j + 1] != NULL) {
                if (process_array[j]->total_cpu_time < process_array[j + 1]->total_cpu_time) {
                    info_list *temp = process_array[j];
                    process_array[j] = process_array[j + 1];
                    process_array[j + 1] = temp;
                }
            }
        }
    }
}

void print_sorted_processes(info_list **process_array, int count) {
    printf("Sorted Process Information (by total CPU time):\n");
    for (int i = 0; i < count; i++) {
        if (process_array[i] != NULL) {
            printf("PID: %d, Name: %s, Total CPU Time: %ld\n",
                   process_array[i]->pid, process_array[i]->name,
                   process_array[i]->total_cpu_time);
        }
    }
}



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

        info_list *process_array[MAX_CPU_PROCESSES] = { NULL };
        printf("Segment to clear up the allocated process") ;
        read_process_info(process_array);
        //print_process_info(process_array);
        sort_process_array(info_list **process_array, int count) ;
        printf("Top Two Processes by Total CPU Time:\n");
        for (int i = 0; i < 2; i++)
        {
            if (process_array[i] != NULL) 
            {
                printf("PID: %d, Name: %s, Total CPU Time: %ld\n",
                    process_array[i]->pid, process_array[i]->name,
                    process_array[i]->total_cpu_time);
            }
        }

 
        for (int i = 0; i < MAX_CPU_PROCESSES; i++) {
            if (process_array[i]) {
                free(process_array[i]);
            }
        }



        close(client_fd);
    }
}

void read_process_info(info_list **process_info) {
    DIR *dir = opendir("/proc");
    struct dirent *entry;

    if (dir == NULL) {
        printf("Failed to open /proc directory\n");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        int pid = atoi(entry->d_name);
        if (pid > 0) {  
            if (scan_process_file(pid, process_info) == -1) {
                printf("Failed to scan process %d\n", pid);
            }
        }
    }
    closedir(dir);
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
