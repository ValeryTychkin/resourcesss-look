//
//  base_socket.h
//  
//
//  Created by Валерий on 12.08.2023.
//

#ifndef base_socket_h
#define base_socket_h


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>


#define SERVER_FILE_NAME "server.sock.file"
#define QUEUE_SIZE 5
#define BUFFER_SIZE 4096
#define CMD_RAM_INFO "ram_info"


struct sockaddr_un createUnixAddr(char file_name[]);

void closeSocket(int socket_descriptor);

int initSocket(void);

int initSocketAndBind(char file_name[]);


#endif /* base_socket_h */
