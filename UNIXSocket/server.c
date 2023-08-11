#include <unistd.h>
#include <sys/sysctl.h>
#include <mach/mach_host.h>
#include <mach/mach.h>

#include "unix_socket.h"


struct ram_memory_info {
    uint64_t total, active, available, used, wired, inactive, free, speculative;
};


typedef unsigned long int page;


void  buffingResult(char imput_message[], size_t message_size, char buffer[]);

uint64_t getTotalRamMemory(void);

page getPageRamMemorySize(void);

void buffingRamInfoResult(char buffer[]);

int loadRamMemoryInfo(struct ram_memory_info *rm);

void stopRunning(int dump);


int main(void) {
    char buffer[BUFFER_SIZE];  // to store incoming and outgoing messages
    bool can_listen = true;
    
    // add signal to catch ctrl-c
    signal(SIGINT, stopRunning);
    
    // create socket
    int server_socket_d = initSocketAndBind(SERVER_FILE_NAME);
    if (server_socket_d == -1) {
        perror("Faled init and bind socket\n");
        can_listen = false;
    }
    
    // add listen rules
    printf("Starting listenin\n");
    if (listen(server_socket_d, QUEUE_SIZE) == -1) {
        perror("Failed start listen\n");
        can_listen = false;
    }
    
    if (can_listen) {
        printf("Creating and starting client socket\n");
    }
    while (can_listen) {
        bool can_accept = true;
        
        // turn ON income connections without checking client (by this reason NULL in arguments)
        int client_socket_d = accept(server_socket_d, NULL, NULL);
        if (client_socket_d == -1) {
            perror("Filed create client socket\n");
        }
        
        do {
            // get request from client and save request message in buffer
            long int bytes_read = recv(client_socket_d, buffer, BUFFER_SIZE, 0);
            if(bytes_read == -1) {
                continue;
            } else {
                printf("Getted message: \"%s\"\n", buffer);
            }
            char message[strlen(buffer)+1];
            // copy input message to another variable
            strcpy(message, buffer);
            // clear buffer
            memset(buffer, 0, BUFFER_SIZE);
            // find answer and write in buffer
            buffingResult(message, sizeof(message), buffer);
            // send result
            printf("Sending message: \"%s\"\n", buffer);
            if (send(client_socket_d, buffer, BUFFER_SIZE, 0) == -1) {
                printf("Failed send message\n");
                continue;
            }
        } while (false);
        // clear buffer and close client socket
        memset(buffer, 0, BUFFER_SIZE);
        closeSocket(client_socket_d);
    }
    
    closeSocket(server_socket_d);
    remove(SERVER_FILE_NAME);
    return 0;
}


void stopRunning(int signum) {
    printf("\nStoping process\n");
    remove(SERVER_FILE_NAME);
    exit(signum);
}



void buffingResult(char imput_message[], size_t message_size, char buffer[]) {
    char command [message_size];
    strcpy(command, imput_message);
    
    if (strcmp(command, CMD_RAM_INFO) == 0) {
        buffingRamInfoResult(buffer);
    } else {
        strcpy(buffer, "{\"err\": \"NotFound\"}");
    }
}


void buffingRamInfoResult(char buffer[]) {
    struct ram_memory_info rm;
    char result[BUFFER_SIZE];
    char error_msg[BUFFER_SIZE] = "";
    
    if (loadRamMemoryInfo(&rm) != 1) {
        strcpy(error_msg, "Failed load ram info intro struct");
        perror(error_msg);
    }
    sprintf(result,"{\"available\":%lld, \"usage\": %lld, \"total\": %lld}, \"err\": \"%s\"",
            rm.available, rm.used, rm.total, error_msg);
    strcpy(buffer, result);
}


int loadRamMemoryInfo(struct ram_memory_info *rm) {
    // TODO add other os support (now only unix)
    uint64_t total = getTotalRamMemory();
    page page_size = getPageRamMemorySize();
    
    // saving statistic by virtual memory in vm (vm_statistics_data_t)
    vm_statistics_data_t vm_statistic;
    mach_port_t mport = mach_host_self();
    mach_msg_type_number_t count = sizeof(vm_statistic) / sizeof(integer_t);
    
    kern_return_t load_statistic_result = host_statistics(mport, HOST_VM_INFO, (host_info_t)&vm_statistic, &count);
    
    //if error to save statistic need return 0, blank rm (ram_memory_info) and print error msg
    if (load_statistic_result != KERN_SUCCESS) {
        char error_str[BUFFER_SIZE];
        sprintf(error_str, "host_statistics(HOST_VM_INFO) syscall failed: %s", mach_error_string(load_statistic_result));
        perror(error_str);
        return 0;
    }
    
    // saving statistic by virtual memory in structure
    rm->total = total;
    rm->wired = vm_statistic.wire_count * page_size;
    rm->inactive = vm_statistic.inactive_count * page_size;
    rm->free = vm_statistic.free_count * page_size;
    rm->active = vm_statistic.active_count * page_size;
    rm->speculative = vm_statistic.speculative_count * page_size;
    // counting informations about VM statistic that are not getting by HOST_VM_INFO
    rm->available = rm->free + rm->inactive;
    rm->used = rm->active + rm->wired;
    rm->free -= rm->speculative;
    
    return  1;
}


uint64_t getTotalRamMemory(void) {
    // TODO add other os support (now only unix)
    page total_pages = sysconf(_SC_PHYS_PAGES);
    page page_size = getPageRamMemorySize();
    return total_pages * page_size;
}


page getPageRamMemorySize(void) {
    // TODO add other os support (now only unix)
    return sysconf(_SC_PAGE_SIZE);
}
