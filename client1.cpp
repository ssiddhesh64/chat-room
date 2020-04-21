/* This program takes 1 argument
    1. Port no
    run as ./server portno
*/

#include<stdio.h>
#include<unistd.h>
#include<bits/stdc++.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string>

#include <arpa/inet.h>


#define BUFFER_LEN 2048
using namespace std;

typedef struct {
    struct sockaddr_in address;
    int sock_fd;
    // int uid;
} client_info;

void send_handler(client_info* cli){

    string msg;

    while(1){
        // cout << "Client1: ";
        getline(cin, msg);
        send(cli->sock_fd, msg.c_str(), msg.length(), 0);
        if(strcmp(msg.c_str(), "bye")==0) break;
    }

    close(cli->sock_fd);
}

void recv_handler(client_info* cli){

    int recv_fd = cli->sock_fd;
    char buffer[BUFFER_LEN];
    bzero(buffer, BUFFER_LEN);
    while(1){
        
        bzero(buffer, BUFFER_LEN);
        int receive = recv(cli->sock_fd, buffer, BUFFER_LEN, 0);
        buffer[receive] = '\0';
        if(receive == 0) break;
        if(strcmp(buffer, "bye")==0){
            break;
        }
        printf("Client2: %s\n", buffer);
        
    } 

    close(recv_fd);
}

int main(int argc, char** argv){

    int port = stoi(argv[1]);
    int socket_fd, new_fd;
    struct sockaddr_in serv_address, cli_address;
    
    string msg;

    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket opening failed");
        exit(EXIT_FAILURE);
    }
    memset(&serv_address, '\0', sizeof(serv_address));
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(port);
    serv_address.sin_addr.s_addr = INADDR_ANY;

    if(bind(socket_fd, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0){
        perror("bind failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    if((listen(socket_fd, 5))<0){
        perror("Listening failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    cout << "Getting in while loop" << endl; 
    while(1){

        cout << "Listening" << endl;
        if((new_fd = accept(socket_fd, (struct sockaddr *) &cli_address, (socklen_t*)&cli_address)) < 0){
            perror("Accept failed");
            close(socket_fd);
            exit(EXIT_FAILURE);
        }
        cout << "Connection accepted" << endl;

        // Client info
        client_info *cli = (client_info *)malloc(sizeof(client_info*));
        cli->address = cli_address;
        cli->sock_fd = new_fd;

        thread printer_thread(recv_handler, cli);
        printer_thread.detach();


        thread sender_thread(send_handler, cli);
        sender_thread.detach();    
    }
    
    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);
    shutdown(new_fd, SHUT_RDWR);
    close(new_fd);
    return 0;
}
