#include <stdio.h>
#include <pthread.h> 
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <datahandler.h>
#include <processhandler.h>
/*  
*   Server side C program to demonstrate Socket programming
*   This code is a modification obtained at the website https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
*   Creator: Skrew Everything, Mar 15, 2018.
*/

#define TRUE 1
#define FALSE 0
#define MAX_RESPONSE_SIZE 41943040

// Variables for all file descriptors given by the operating system.
int server_fd, new_socket, html_fd;
// Struct with all network setting data.
struct sockaddr_in address;
// Size of the struct in bytes.
int addrlen = sizeof(address);
// Process counter
int process_id = 0;
// Main server thread.
pthread_t server_thread, process_thread;
// Mutex struct.
pthread_mutex_t lock;
// Client response memory variables. 
char * response;
char * temp_response;
char * response_data;
// Configuration file string data.
char * conf_data;
// Configuration struct. This has all configuration file usefull data.
conf info;
// List for process management.
process_node process_list;
char * time_s;

void init(){
    // Reading configuration file.
    setConfigurationFileData(&info);
    // Creating socket file descriptor. If the syscall socket returns 0 there is an error.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    //This is to prevent the "In bind: Address already in use" error.
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int)) < 0){
        perror("setsockopt(SO_REUSEPORT) failed");
        exit(EXIT_FAILURE);
    }
    // Setting all network values in address struct.
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(info.port);
    // Set all memory spaces with \0 value in sin_zero struct attribute.
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    // Stablishing socket connection with bind system call. If the syscall bind returns less than 0 there is an error.
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    // Telling operating system to listen new connection requests for the socket. If the syscall listen returns less than 0 there is an error.
    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    // Reserving memory to avoid segmentation fault when the server is suddenly closed.
    response = (char *) calloc(1, sizeof(char));
    temp_response = (char *) calloc(1, sizeof(char));
}

void * run(void *ptr){
    printf("\n------------------ Waiting for new connection ------------------\n\n");
    // Server endless while. It continues forever while there is no error at accepting the client connection request. If the syscall accept returns less than 0 there is an error.
    while((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) > 0) {
        // Releasing response memory to avoid segmentation fault.
        free(response);
        free(temp_response);
        // Reserving memory for client response.
        char * response = (char *) calloc(MAX_RESPONSE_SIZE, sizeof(char));
        // This response helps to read the socket more than once because the data is not being completely read.
        char * temp_response = (char *) calloc(MAX_RESPONSE_SIZE/4, sizeof(char));
        // Reading client response and storing that data into the response.
        read(new_socket, response, MAX_RESPONSE_SIZE);
        // Using the function getResponseProperty we get the content length from the response
        char * request_size_str = getProperty("content-length", response);
        // Check if content length property exists
        if(request_size_str != NULL){
            // Turning content length to integer
            int request_size = atoi(request_size_str);
            // If the content read by the response is less than the content lenght, we read again the socket until we get the whole response
            while(strlen(response) <= request_size){
                read(new_socket, temp_response, MAX_RESPONSE_SIZE/4);
                strcat(response, temp_response);
                memset(temp_response, 0, MAX_RESPONSE_SIZE/4);
            }
        }
        //printf("%s\n",response);
        char * client = getProperty("user-agent", response);
        char * file = getProperty("name", response);
        char * img = getProperty("img", response);

        writeLog("log.file", client, file, time_s, "pending");
        insertProcess(&process_list, ++process_id, client, file, time_s, img);
        /*
        process_node aux = process_list;
        do{
            printf("%s\n", aux->client);
            printf("%s\n", aux->file);
        }while((aux = aux->next) != NULL);*/

        printf("\n------------------ Client connection finished -------------------\n\n");
        printf("\n------------------ Waiting for new connection ------------------\n\n");
        // Closing the socket.
        close(new_socket);
    }
}

void * processing(void *ptr){
    while(TRUE){
        time_s = getCurrentTime();
        
    }
}


void start(){
    // Initializing all server variables
    init();
    // Main server thread initialization
    pthread_create(&server_thread,  NULL, run, NULL);
    pthread_create(&process_thread, NULL, processing, NULL);
}

void stop(){
    // Close the sever thread
    pthread_cancel(server_thread);
    pthread_cancel(process_thread);
    // Closing the used sockets.
    close(new_socket);
    close(server_fd);
}

void startServer(){
    start();
    printf("Server running on port %d\n", info.port);
    printf("Http Server has started\n");
}

void stopServer(){
    stop();
    printf("Http Server has stoped\n");
}