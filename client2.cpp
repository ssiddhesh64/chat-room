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

bool flag = false;

typedef struct {
    struct sockaddr_in address;
    int sock_fd;
    // int uid;
} client_info;

void send_handler(client_info* cli){

    string msg;

    while(1){
        // cout << "Client2: ";
        getline(cin, msg);
        send(cli->sock_fd, msg.c_str(), msg.length(), 0);
        if(strcmp(msg.c_str(), "bye")==0) break;
    }
    flag = true;
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
        printf("Client1: %s\n", buffer);
        
    } 
    flag = true;
    close(recv_fd);
}

int main(int argc, char** argv){

    int port = stoi(argv[1]);
    int socket_fd;
    struct sockaddr_in serv_addr;
    // struct hostent *server;
    // char buffer[1024] = {0};
    //char msg[1024];
    // string msg;

    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket opening failed");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, '\0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    
    // cout << "IP address: " << serv_addr.sin_addr.s_addr << endl;

    if(connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("connect failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    cout << "Connected" << endl;
    client_info *cli = (client_info *)malloc(sizeof(client_info*));
    cli->address = serv_addr;
    cli->sock_fd = socket_fd;
    // while(1){
        
        // if(flag) break;
        thread sender_thread(send_handler, cli); 
        // sender_thread.detach();

        thread printer_thread(recv_handler, cli);
        // printer_thread.detach();        
            
    // }
    sender_thread.join();
    printer_thread.join();
    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);
    return 0;
}
