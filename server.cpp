// Server side C/C++ program to demonstrate Socket programming 
// ports: Peter 12117, Jara 12115, Mark 12104
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <iostream>
#define PORT 12104
using namespace std;

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket;
    // int valread;
    struct sockaddr_in address;
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};
    // const char *HELLO = "Hello from server";
    char HELLO[1024] = "Hello from server"; 
    char DISCONNECT_RPC[1024] = "disconnect"; 
    char DISCONNECT_MSG[1024] = {0}; 
    
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY;

    // htons = host to network short. Accounts for computer-side short storage peculiarities.
    address.sin_port = htons( PORT );
    
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address, 
                                sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    // this loop is the server remaining active 
    bool disconnectFlag = false;
    while (true) {
        // cout << "You're in the outer while-loop" << endl;
        if (listen(server_fd, 3) < 0)
        { 
            cout << endl << "Listen error" << endl;
            perror("listen");
            exit(EXIT_FAILURE); 
        } 
        // new_socket is where we'll be communicating to the client
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                        (socklen_t*)&addrlen))<0) 
        { 
            cout << endl << "accept error" << endl;
            perror("accept");
            exit(EXIT_FAILURE); 
        }

        // inner while loop
        // FIXME remove max 10
        while (!disconnectFlag) {

            // valread = (would equal size of message)
            read(new_socket, buffer, 1024);

            if (*buffer == *DISCONNECT_RPC) {
                send(new_socket , DISCONNECT_MSG , strlen(buffer) , 0 ); 
                disconnectFlag = true;
                
                // close active socket
                close(new_socket);
            } else {
                // message sent back to client.
                send(new_socket , buffer , strlen(buffer) , 0 ); 
            }
            // TODO test if this is necessary
            memset(buffer, 0, sizeof(buffer));
        }


        disconnectFlag = false;
    }

    // printf("%s, valread = %d\n",buffer, valread);
    send(new_socket , HELLO , strlen(HELLO) , 0 ); 
    // printf("Hello message sent\n");

    return 0; 
} 

