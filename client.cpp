// Client side C/C++ program to demonstrate Socket programming 
// ports: Peter 12117, Jara 12115, Mark 12104
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <iostream>
#define PORT 12117
using namespace std;

int main(int argc, char** argv) { 

    if (argc != 3) {
        cout << "WARNING: please enter username and password into CLI, separated by a space." << endl;
        cout << "Recommended: user=mike, p/w=123" << endl;
    }
    srand (time(NULL));    
    const int SLEEP_TIME = rand() % 10 + 1;  // Sleep for 1 to 10 seconds

    int sock = 0;
    struct sockaddr_in serv_addr; 
    char buffer[1024] = {0}; 
    strcpy(buffer, "rpc=connect;");  // take note: this is strcpy, not strcat! (easy to overlook)
    strcat(buffer, "user=");
    strcat(buffer, argv[1]);
    strcat(buffer, ";password=");
    strcat(buffer, argv[2]);
    strcat(buffer, ";");
    puts(buffer);  // another way to print to screen

    //const char *CONNECT_RPC = "rpc=connect;user=mike;password=123;";
    //const char *PWD_FAILURE_RPC = "rpc=connect;user=mike;password=WRONG_PW;";     
    //const char *USSR_FAILURE_RPC = "rpc=connect;user=miky;password=123;";     
    const char *DISCONNECT_RPC = "disconnected"; 
    // char DISCONNECT_MSG[1024] = {0}; 
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 

    // connects the socket (referred to by the file descriptor sockfd)
    // to the address specified by addr. (Server’s address and port is specified in addr).
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }

    // sending username/password rpc
    send(sock , buffer , strlen(buffer) , 0 );

    // TODO: please fix! should not always print!
    // int statusCode = 0; // 0 = no error
    // if (statusCode == 0)
    // 	printf("\nWelcome %s", argv[1]);

    // read incoming message
    read(sock, buffer, 1024); // remember: read returns an int, corresponding the number of characters entered
    // cout << "THis is the thing" << buffer << endl;
    if (strcmp(buffer, DISCONNECT_RPC) == 0) {
        return 0;
    } else {
        /*
        cout << "============" << endl;
        cout << "buffer:" <<  buffer << endl;
        cout << "============" << endl;
        cout << "DISCONNECT_RPC: " << DISCONNECT_RPC << endl;
        */
    }

    // sleep timer
    cout << "Auto-disconnect in " << SLEEP_TIME << " seconds";
    cout.flush();
    sleep(SLEEP_TIME);
    cout << endl;

    // server is in listening loop
    send(sock , DISCONNECT_RPC , strlen(DISCONNECT_RPC) , 0 );
    // disconnect message sent

    read(sock, buffer, 1024);

    return 0; 
} 

