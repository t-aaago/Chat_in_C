#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 20
#define MAX_MSG 20
#define PORT 8888

int clients[MAX_CLIENTS];  
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void broadcast(const char* message, int client_fd){
    for (size_t i = 0; i < MAX_CLIENTS; i++){

        if(clients[i] != 0 && clients[i] != client_fd){
            send(clients[i], message, strlen(message), 0);
        }

    }
}

void handleClient(void* arg){
    int client_fd = *(int*) arg;
    char buffer[MAX_MSG];
    ssize_t len = 0;

    while((len = recv(client_fd, buffer, sizeof(buffer), 0)) > 0){
        broadcast(buffer, client_fd);
    }
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

        pthread_mutex_lock(&lock);
        for (size_t i = 0; i < MAX_CLIENTS; i++){
            if(clients[i] == 0){
                clients[i] = client_fd;
            }
        }
        pthread_mutex_unlock(&lock);
        
        pthread_create(&client_addr, NULL, handleClient, &client_fd);
        pthread_detach(client_thread);
    }
    
    close(server_fd);
    print("Server Finished!\n");
    return 0;
}