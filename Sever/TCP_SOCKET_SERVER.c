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

#define PORT 1717

int main(int argc, char const *argv[])
{
    // Variables for all file descriptors given by the operating system.
    int server_fd, new_socket, html_fd;
    // Struct with all network setting data.
    struct sockaddr_in address;
    // Size of the struct in bytes.
    int addrlen = sizeof(address);

    char request[10240], html[9216], str[12]; //this needs to be placed in a function apart <--------------------------------------------

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

    // Reserving memory to avoid segmentation fault when the server is suddenly closed.
    char * buffer = (char *) calloc(1, sizeof(char));

    // Endless while. This need to be placed inside a thread. <--------------------------------------------
    while(1) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        // Releasing buffer memory to avoid segmentation fault.
        free(buffer);
        // Openning the html document in read-only mode.
        int html_fd = open("index.html", O_RDONLY); 
        // Reding the whole html document.
        read(html_fd, html, 9216); 
        // Adding the headder of the http request to the request buffer. 
        strcpy(request, "HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\nContent-Length: ");
        // Concatenating the size of the html code. 
        sprintf(str, "%ld", strlen(html));
        strcat(request, str);
        // Concatenating the html code for being sent to the server. 
        strcat(request, "\n\n");
        strcat(request, html);
        // Closing the html code file.
        close(html_fd);
        // Accepting the client connection request. If the syscall accept returns less than 0 there is an error.
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        // Reserving memory for client response.
        char * buffer = (char *) calloc(41943040, sizeof(char));
        // Reading client response and storing that data into the buffer.
        read(new_socket , buffer, 41943040);
        printf("%s\n",buffer);
        // Sending request from server to the client.
        write(new_socket ,request ,strlen(request));

        printf("------------------Data has been sent to the client-------------------\n");
        // Resetting all arrays values.
        memset(request, 0, sizeof(request));
        memset(html,    0, sizeof(html));
        memset(str,     0, sizeof(str));
        // Closing the socket.
        close(new_socket);
    }

    return 0;
}
