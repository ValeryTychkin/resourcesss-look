#include "unix_socket.h"


void connectAndSend(char message[], int message_size, char buffer[]);


int getRamInfo(char buffer[]);


int main(void) {
    printf("Start testing sending");
    char *buffer = malloc(BUFFER_SIZE);
    char message[] = "Incorrect cmd str";
    connectAndSend(message, sizeof(message), buffer);
    memset(buffer, 0, BUFFER_SIZE);
    getRamInfo(buffer);
    printf("Free buffer (str in buffer: \"%s\")\n", buffer);
    free(buffer);
}


int connectToServer(void) {
    bool need_close = false;
    int socket_d = initSocket();
    
    struct sockaddr_un socket_addr = createUnixAddr(SERVER_FILE_NAME);
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


void connectAndSend(char message[], int message_size, char buffer[]) {
    int socket_d = connectToServer();
    sendRequest(socket_d, message, message_size, buffer);
}


int getRamInfo(char buffer[]) {
    char command[] = CMD_RAM_INFO;
    connectAndSend(command, sizeof(command), buffer);
    return *buffer;
}
