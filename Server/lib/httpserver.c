/*  
*   Server side C program to demonstrate Socket programming
*   This code is a modification obtained at the website https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
*   Creator: Skrew Everything, Mar 15, 2018.
*/
#include <stdio.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <jsonhandler.h>

#define PORT 1717
#define MAX_BUFFER_SIZE 41943040

// Variables for all file descriptors given by the operating system.
int server_fd, new_socket, html_fd;
// Struct with all network setting data.
struct sockaddr_in address;
// Size of the struct in bytes.
int addrlen = sizeof(address);

void initServer(){
    // Creating socket file descriptor. If the syscall socket returns 0 there is an error.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    // Setting all network values in address struct.
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
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
}

void startServer(){
	// Reserving memory to avoid segmentation fault when the server is suddenly closed.
    char * buffer = (char *) calloc(1, sizeof(char));

    // Endless while. This need to be placed inside a thread. <--------------------------------------------
    while(1) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        // Releasing buffer memory to avoid segmentation fault.
        free(buffer);
        // Accepting the client connection request. If the syscall accept returns less than 0 there is an error.
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        // Reserving memory for client response.
        char * buffer = (char *) calloc(MAX_BUFFER_SIZE, sizeof(char));
        // This buffer helps to read the socket more than once because the data is not being completely read.
        char * temp_buffer = (char *) calloc(MAX_BUFFER_SIZE/4, sizeof(char));
        // Reading client response and storing that data into the buffer.
        read(new_socket, buffer, MAX_BUFFER_SIZE);
        // Using the function getResponseProperty we get the content length from the response
        char * request_size_str = getResponseProperty("Content-Length: ", buffer); 
        // Check if content length property exists
        if(request_size_str != NULL){
            // Turning content length to integer
            int request_size = atoi(request_size_str);
            // If the content read by the buffer is less than the content lenght, we read again the socket until we get the whole response
            while(strlen(buffer) < request_size){
                read(new_socket, temp_buffer, MAX_BUFFER_SIZE/4);
                strcat(buffer, temp_buffer);
                memset(temp_buffer, 0, MAX_BUFFER_SIZE/4);
            }
        }

        printf("%s\n",buffer);
        printf("------------------Data has been sent to the client-------------------\n");

        // Closing the socket.
        close(new_socket);
    }
}