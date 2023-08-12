//
//  base_socket.c
//  
//
//  Created by Валерий on 12.08.2023.
//
#include <sys/stat.h>

#include "base_socket.h"

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
    // TODO NEED CHANGE FORM FILE ON IP:PORT!!!! Can't connect by file fom docker image to server through the docker volume
    bool need_close_socket = false;
    
    int socket_d = initSocket();
    
    struct sockaddr_un socket_addr = createUnixAddr(file_name);
    
    unlink(file_name);
    
    printf("Binding socket server\n");
    int socket_bind = bind(socket_d, (struct sockaddr *)&socket_addr, sizeof(socket_addr));
    if (socket_bind == -1) {
        perror("Faled socket bind\n");
        need_close_socket = true;
    }
    
    
    chmod(socket_addr.sun_path, S_IFMT|S_IRWXU|S_IRWXG|S_IRWXO);
    
    if (need_close_socket) {
        closeSocket(socket_d);
        return -1;
    }
    return socket_d;
}

