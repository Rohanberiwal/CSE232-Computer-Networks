#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h> 
#include <pthread.h>
#define MAX_TOP_PROCESSES 2
long get_clock_ticks_per_second() {
    static long clock_ticks = 0;
    if (clock_ticks == 0) {
        clock_ticks = sysconf(_SC_CLK_TCK);
    }
    return clock_ticks;
}

typedef struct {
    char name[256];    // Process name
    int pid;           // Process ID
    long user_time;    // User CPU time
    long kernel_time;  // Kernel CPU time
    long total_time;   // Total CPU time (user + kernel)
} ProcessInfo;


#define PORT 8081
#define MAX_EVENTS 100
#define MAX_CPU_PROCESSES 1024
#define BUFFER_SIZE 2048
#define MAX_CONNECTIONS 10
#define EXTRA_FILE "extra.txt"

int count_user  = 0 ;

void clear_extra_file() {
    FILE *file = fopen(EXTRA_FILE, "w");
    if (file) 
    {
        fclose(file);
        //printf("FIle is cleaned ") ;
    }
    else 
    {
        printf("Failed to clear extra.txt");
    }
}

// Function to write process information to a file
void write_process_info(const char *name, int pid, long user_time, long kernel_time)
{
    clear_extra_file();
    FILE *file = fopen(EXTRA_FILE, "a"); 
    if (file == NULL) {
        printf("Could not open extra.txt for writing");
        return;
    }

    fprintf(file, "Process Name: %s\n", name);
    fprintf(file, "PID: %d\n", pid);
    fprintf(file, "User Time: %ld\n", user_time);
    fprintf(file, "Kernel Time: %ld\n\n", kernel_time);
    fclose(file);
}

// Function to read a process's information from its stat file
int read_process_stat(const char *pid, char *name, long *utime, long *stime) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%s/stat", pid);
    
    FILE *file = fopen(path, "r");
    if (file) {
        // Read relevant fields: pid, command name, user time, and kernel time
        fscanf(file, "%*d %s %*c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %ld %ld",
               name, utime, stime);
        fclose(file);
        return 1; // Successful read
    }
    return 0; 
}




#include <string.h>

// Function to find the top two CPU-consuming processes
void process_finder(long total_cpu_time, const char *process_name, int pid,
                    long user_time, long kernel_time,
                    long max_cpu_time[2], char top_process_name[2][256], 
                    int top_pid[2], long top_user_time[2], long top_kernel_time[2]) {
    
    // Check if this process is a new top CPU consumer
    if (total_cpu_time > max_cpu_time[0]) {
        // Shift the current first to second
        max_cpu_time[1] = max_cpu_time[0];
        strcpy(top_process_name[1], top_process_name[0]);
        top_pid[1] = top_pid[0];
        top_user_time[1] = top_user_time[0];
        top_kernel_time[1] = top_kernel_time[0];

        // Update the first max
        max_cpu_time[0] = total_cpu_time;
        strcpy(top_process_name[0], process_name);
        top_pid[0] = pid;
        top_user_time[0] = user_time;
        top_kernel_time[0] = kernel_time;
    } else if (total_cpu_time > max_cpu_time[1]) {

        max_cpu_time[1] = total_cpu_time;
        strcpy(top_process_name[1], process_name);
        top_pid[1] = pid;
        top_user_time[1] = user_time;
        top_kernel_time[1] = kernel_time;
    }
}

void scan_processes() {
    // Clear the contents of extra.txt before writing new data
    clear_extra_file();
    
    DIR *dir;
    struct dirent *entry;
    long max_cpu_time[2] = {0, 0}; // Array to hold max CPU times for top two
    char top_process_name[2][256] = {"", ""}; // Array to hold names of top two processes
    int top_pid[2] = {-1, -1}; // Array to hold PIDs of top two processes
    long top_user_time[2] = {0, 0}; // Array to hold user times for top two processes
    long top_kernel_time[2] = {0, 0}; // Array to hold kernel times for top two processes

    if ((dir = opendir("/proc")) == NULL) {
        perror("Could not open /proc directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Only consider directories with numeric names (process IDs)
        if (isdigit(entry->d_name[0])) {
            char process_name[256];
            long user_time, kernel_time;

            if (read_process_stat(entry->d_name, process_name, &user_time, &kernel_time)) {
                long total_cpu_time = user_time + kernel_time;
                process_finder(total_cpu_time, process_name, atoi(entry->d_name),
                                user_time, kernel_time, max_cpu_time,
                                top_process_name, top_pid,
                                top_user_time, top_kernel_time);
            }
        }
    }
    closedir(dir);
    for (int i = 0; i < 2; i++) {
        if (top_pid[i] != -1) {
            write_process_info(top_process_name[i], top_pid[i], top_user_time[i], top_kernel_time[i]);
        }
    }

    if (top_pid[0] == -1) {
        printf("No processes found.\n");
    }
}

void *handle_client(void *arg) {
    int new_socket = *(int *)arg;
    free(arg); 
    char buffer[1024] = {0};
    const char *hello = "Hello from server";

    ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        printf("Read error");
        close(new_socket);
        return NULL;
    } else if (bytes_read == 0) {
        printf("Client disconnected unexpectedly\n");
        close(new_socket);
        return NULL;
    }

    // Process the request
    scan_processes();
    ssize_t bytes_sent = send(new_socket, "hello", strlen(hello), 0);
    if (bytes_sent < 0) {
        printf("Send error");
        close(new_socket);
    }

    printf("Message sent to client: %s\n", hello);
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
