#include "client.h"


void connectAndSend(char socket_file_path[], char message[], int message_size, char buffer[]);


int getRamInfo(char socket_file_path[], char buffer[]);


int main(void) {
    printf("Start testing sending");
    char *buffer = malloc(BUFFER_SIZE);
    char message[] = "Incorrect cmd str";
    connectAndSend(SERVER_FILE_NAME, message, sizeof(message), buffer);
    memset(buffer, 0, BUFFER_SIZE);
    getRamInfo(SERVER_FILE_NAME, buffer);
    printf("Free buffer (str in buffer: \"%s\")\n", buffer);
    free(buffer);
}


int connectToServer(char socket_file_path[]) {
    bool need_close = false;
    int socket_d = initSocket();
    
    struct sockaddr_un socket_addr = createUnixAddr(socket_file_path);
    printf("Connecting to socket server\n");
    if (connect(socket_d, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) == -1) {
        perror("Failed client connection\n");
        need_close = true;
    }
    return socket_d;
}


void sendRequest(int socket_d, char message[], int message_size, char buffer[]) {
    printf("Sending message: %s\n", message);
    long int send_status = send(socket_d, message, message_size, 0);
    memset(buffer, 0, BUFFER_SIZE);
    recv(socket_d, buffer, BUFFER_SIZE, 0);
    printf("Getted response and save in buffer: \"%s\"\n", buffer);
}


void connectAndSend(char socket_file_path[], char message[], int message_size, char buffer[]) {
    int socket_d = connectToServer(socket_file_path);
    sendRequest(socket_d, message, message_size, buffer);
}


int getRamInfo(char socket_file_path[], char buffer[]) {
    char command[] = CMD_RAM_INFO;
    connectAndSend(socket_file_path, command, sizeof(command), buffer);
    return *buffer;
}
