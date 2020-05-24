// Server side C/C++ program to demonstrate Socket programming 
// ports: Peter 12117, Jara 12115, Mark 12104
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <iostream>
#include "MainMenu.cpp"
#include "StringParser.h"

// next line from MM
// #include "assert.h"
#define PORT 8080
using namespace std;

//TODO check this for mutex lock
// prepares a way for a thread to be sent to a dynamically created menu.
void *threadToMenu(void *arg) {
    int *socketPtr = (int *) arg;
    MainMenu *mainMenuPtr;
    mainMenuPtr = new MainMenu(*socketPtr);
    mainMenuPtr->loop();
    delete mainMenuPtr;
    pthread_exit(NULL);
    // don't need to return null in a void method
}

// Connects the client socket to the server socket
// TODO candidate
// return 0 if password/username passed.
// return -2 if incorrect password
// return -1 if username does not exist
int passwordVaultStub(char *username, char *password) {
    const char *CORRECT_UN = "mike";
    const char *CORRECT_PW = "123";

    if (strcmp(username, CORRECT_UN) == 0) {
        if (strcmp(password, CORRECT_PW) == 0)
            return 0;
        else
            return -2;
    } else
        return -1;
}

int authenticate(char *buffer, StringParser &parser) {
    KeyValue authMessenger;
    KeyValue usernameMessenger;
    KeyValue passwordMessenger;
    parser.newRPC(buffer);
    parser.getNextKeyValue(authMessenger);
    parser.getNextKeyValue(usernameMessenger);
    parser.getNextKeyValue(passwordMessenger);

    if ((strcmp(authMessenger.getKey(), "rpc") != 0) || 
        (strcmp(authMessenger.getValue(), "connect") != 0))
        {
            return -3;
        }

    char *username = usernameMessenger.getValue();
    char *password = passwordMessenger.getValue();

    // returns a -1 for bad username, -2 for bad password, 0 if passed.
    int retValue = passwordVaultStub(username, password);
    cout << retValue << endl;
    return retValue;
}


// Sends a message to client, and then closes the socket assigned to current client.
// return 0 if successful 
// return -1 if failed
int disconnect(int socket_num, char *buff){
    // char disconnectMsg[1024] = {0};
    send(socket_num, buff, strlen(buff) , 0 ); 
    // close active socket
    return close(socket_num);
}

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};
    char DISCONNECT_RPC[1024] = "rpc=disconnect;"; 
    

    StringParser *parser = new StringParser; 
    KeyValue rpcKeyValue;

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                             &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );       // htons = host to network short. Accounts for computer-side short storage peculiarities.
    
    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, 
                                sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
            

    // TODO Upgrade this when threads are dynamically created
    pthread_t singleThread;

    // this loop is the server remaining active
    while (true) {
        cout << "Waiting for connection" << endl << "... ... ... ..." << endl;
        
        // Establish a new connection.
        if (listen(server_fd, 3) < 0) {
            cout << endl << "Listen error" << endl;
            perror("listen");
            exit(EXIT_FAILURE); 
        } 
        // new_socket is where we'll be communicating to the client
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                            (socklen_t*)&addrlen))<0) { 
            cout << endl << "accept error" << endl;
            perror("accept");
            exit(EXIT_FAILURE); 
        }

        // Username and password RPC should be sent in here.
        read(new_socket, buffer, 1024);

        /* A password is sent into this, and out of it comes a response: password is good or bad.*/
        int connectReturn = 0;
        connectReturn = authenticate(buffer, *parser);
        cout << "Login result: " << connectReturn << endl;

        if (connectReturn < 0) {
            connectReturn = disconnect(new_socket, DISCONNECT_RPC);
        } else {
            send(new_socket , buffer , strlen(buffer) , 0 );
        }

        // In a function create dynamic mainmenu, and populate it with a single thread
        pthread_create(&singleThread, NULL, threadToMenu, (void *) &new_socket);

        cout << "Thread created, returning to listening state" << endl;
    }


    return 0; 
}