#ifndef unix_socket_h
#define unix_socket_h


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>


#define CLIENT_FILE_NAME "client.sock"
#define SERVER_FILE_NAME "server.sock"
#define QUEUE_SIZE 5
#define BUFFER_SIZE 4096
#define CMD_RAM_INFO "ram_info"


struct sockaddr_un createUnixAddr(char file_name[]) {
    printf("Creating structur of socket addres (file_name: \"%s\")\n", file_name);
    struct sockaddr_un socket_addr;
    socket_addr.sun_family = AF_UNIX;
    strcpy(socket_addr.sun_path, file_name);
    return socket_addr;
}


void closeSocket(int socket_descriptor) {
    if (socket_descriptor != -1) {
        printf("Closing socket\n");
        close(socket_descriptor);
    }
}


int initSocket(void) {
    printf("Creating socket\n");
    int socket_d = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_d == -1) {
        perror("Faled create socket\n");
    }
    return socket_d;
}


int initSocketAndBind(char file_name[]) {
    bool need_close_socket = false;
    
    int socket_d = initSocket();
    
    struct sockaddr_un socket_addr = createUnixAddr(file_name);
    
    printf("Binding socket server\n");
    int socket_bind = bind(socket_d, (struct sockaddr *)&socket_addr, sizeof(socket_addr));
    if (socket_bind == -1) {
        perror("Faled socket bind\n");
        need_close_socket = true;
    }
    
    if (need_close_socket) {
        closeSocket(socket_d);
        return -1;
    }
    return socket_d;
}

#endif
