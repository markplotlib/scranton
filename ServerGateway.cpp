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
#include "ServerStats.h"
#include "ThreadContext.h"
// next line from MM
// #include "assert.h"
#define PORT 12104
using namespace std;

ServerStats serverStats;

//TODO check this for mutex lock
// Prepares a way for a thread to be sent to a dynamically created menu.
void *threadToMenu(void *arg) {
    int *socketPtr = (int *) arg;
    // stores client context
    ThreadContext *context = new ThreadContext();
    MainMenu *mainMenuPtr;
    mainMenuPtr = new MainMenu(*socketPtr);
    mainMenuPtr->loopThread(serverStats, *context);
    delete context;
    delete mainMenuPtr;
    serverStats.decrementNumActiveClients();
    pthread_exit(NULL);
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

// Authenticates user credentials
// returns a -1 for bad username
// returns -2 for bad password
// returns -3 for bad RPC
// returns 0 if passed
int authenticate(char *buffer, StringParser &parser) {
    KeyValue rpcKV;     // "rpc=[exact_rpc]"
    KeyValue usernameKV;
    KeyValue passwordKV;
    parser.newRPC(buffer);
    parser.getNextKeyValue(rpcKV);
    parser.getNextKeyValue(usernameKV);
    parser.getNextKeyValue(passwordKV);

    if ((strcmp(rpcKV.getKey(), "rpc") != 0) || 
        (strcmp(rpcKV.getValue(), "connect") != 0))
        {
            return -3;  // represents bad rpc
        }

    char *username = usernameKV.getValue();
    char *password = passwordKV.getValue();

    // Returns a -1 for bad username, -2 for bad password, -3 for bad RPC, 0 if passed.
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

// Establishes a socket connection 
int startServer(struct sockaddr_in m_address)
{
    int m_port = PORT;
    int opt = 1;
    int m_server_fd;

    // Creating socket file descriptor 
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // Helps manipulate socket options referred by the file descriptor sockfd. 
    // Helps in reuse of address and port. Prevents error such as: “address already in use”.
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
        &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = (uint16_t) htons((uint16_t) m_port);
    // bind socket to address and port number 
    if (bind(m_server_fd, (struct sockaddr *)&m_address,
        sizeof(m_address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    return m_server_fd;
}

// Runs the server program 
int main(int argc, char const *argv[]) 
{ 
    const int MAX_CLIENTS = 10;
    int server_fd, new_socket;
    struct sockaddr_in m_address;
    // int opt = 1;
    int addrlen = sizeof(m_address); 
    char buffer[1024] = {0};
    char DISCONNECT_RPC[1024] = "rpc=disconnect;"; 

    StringParser *parser = new StringParser; 
    KeyValue rpcKeyValue;

    // Server creates socket file descriptor, sets socket options, and binds socket
    server_fd = startServer(m_address);
    
    // To store thread
    pthread_t singleThread;

    // This loop is the server remaining active
    while (true) {
        cout << "Waiting for connection" << endl << "... ... ... ..." << endl;

        // Establish a new connection.
        if (listen(server_fd, 3) < 0) {
            cout << endl << "Listen error" << endl;
            perror("listen");
            exit(EXIT_FAILURE); 
        }

        // if number of clients exceeds the maximum, then new incoming client will be disconnected.
        if (serverStats.getNumActiveClients() >= MAX_CLIENTS) {
            read(new_socket, buffer, 1024);
            disconnect(new_socket, DISCONNECT_RPC);
        } else {
            // new_socket is where we'll be communicating to the client
            if ((new_socket = accept(server_fd, (struct sockaddr *)&m_address, 
                                (socklen_t*)&addrlen))<0) { 
                cout << endl << "accept error" << endl;
                perror("accept");
                exit(EXIT_FAILURE); 
            }

            // Username and password RPC should be sent in here.
            read(new_socket, buffer, 1024);

            // A password is sent into this, and out of it comes a response: password is good or bad
            int connectReturn = -4; // -4 represents a disconnect (due to too many clients, for example)
            connectReturn = authenticate(buffer, *parser);
            cout << "Login result: " << connectReturn << endl;

            if (connectReturn < 0) {
                // client disconnected due to bad credentials
                connectReturn = disconnect(new_socket, DISCONNECT_RPC);
            } else {
                // client remains connected
                // In a function create dynamic mainmenu, and populate it with a single thread
                // FIXME: have this check for a return from pthread_create, in case a new pthread can't be created.
                serverStats.incrementNumActiveClients();
                pthread_create(&singleThread, NULL, threadToMenu, (void *) &new_socket);
                send(new_socket , buffer , strlen(buffer) , 0 );
            }
            cout << "Thread created, returning to listening state" << endl;
        }
    }
    return 0; 
}