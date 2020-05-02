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
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};
    const char *HELLO = "Hello from server"; 
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

    bool disconnectFlag = false;
    // this loop is the server remaining active 
    while (true) {
        // Listen
        cout << "You're in the outer while-loop" << endl;
        if (listen(server_fd, 3) < 0)
        { 
            cout << endl << "Listen error" << endl;
            perror("listen");
            exit(EXIT_FAILURE); 
        } 
        // creating the temporary socket, by which we're communicating to the server????
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                        (socklen_t*)&addrlen))<0) 
        { 
            cout << endl << "accept error" << endl;
            perror("accept");
            exit(EXIT_FAILURE); 
        }

        int counter = 0;
        // inner while loop
        while (!disconnectFlag) {
            counter++;
            cout << "You're in the INNER while-loop" << endl;

            valread = read(new_socket, buffer, 1024);
            // troubleshooting
            cout << endl << "read value = " << valread << endl;

            // FIXME
            cout << buffer << endl;
            cout << DISCONNECT_RPC << endl;
            if (buffer == DISCONNECT_RPC) {
                cout << "If clause" << endl;

                // TODO, if broken here: buffer = DISCONNECT_MSG?
                send(new_socket , DISCONNECT_MSG , strlen(buffer) , 0 ); 
                disconnectFlag = true;
                
                // close active socket
                close(new_socket);
            } else {
                cout << "Else clause" << endl;
                // stay connected. 
                // RPC executes.
                // message sent back to client.
                send(new_socket , buffer , strlen(buffer) , 0 ); 
            }
        }

        cout << "Inner loop counter: " << DISCONNECT_RPC << counter << endl;

        cout << endl << "DISCONNECTING BUDDY FROM OTHER BUDDY! NO MORE FRIENDS" << endl;
        disconnectFlag = false;
    }

    printf("%s, valread = %d\n",buffer, valread);
    send(new_socket , HELLO , strlen(HELLO) , 0 ); 
    printf("Hello message sent\n");

    return 0; 
} 

