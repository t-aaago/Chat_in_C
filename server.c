#include <stdio.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 20
#define MAX_MSG 20
#define PORT 8888

int clients[MAX_CLIENTS];  

void handleClient(void* arg){
    int client_fd = *(int*) arg;
}

int main(){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    pthread_t client_thread;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, &server_addr, sizeof(server_addr));
    listen(server_fd, MAX_CLIENTS);
    printf("Server up and Listening in %d\n", PORT);

    while ((client_fd  = accept(server_fd, &client_addr, sizeof(client_addr))))
    {
        printf("New client connected!\n");
        pthread_create(&client_addr, NULL, handleClient, &client_fd);
        pthread_detach(client_thread);
    }
    
    close(server_fd);
    print("Server Finished!\n");
    return 0;
}