// Client side C/C++ program to demonstrate Socket programming 
// ports: Peter 12117, Jara 12115, Mark 12104
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <iostream>
#define PORT 12104
using namespace std;

int main(int argc, char const *argv[]) 
{ 
    srand (time(NULL));    
    const int SLEEP_TIME = rand() % 10 + 1;  // Sleep for 1 to 10 seconds

    int sock = 0;
    struct sockaddr_in serv_addr; 
    const char *CONNECT_RPC = "rpc=connect;user=mike;password=123;"; 
    //const char *PWD_FAILURE_RPC = "rpc=connect;user=mike;password=WRONG_PW;";     
    //const char *USSR_FAILURE_RPC = "rpc=connect;user=miky;password=123;";     
    const char *DISCONNECT_RPC = "disconnect"; 
    char buffer[1024] = {0}; 
//    char DISCONNECT_MSG[1024] = {0}; 
    
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

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }

    send(sock , CONNECT_RPC , strlen(CONNECT_RPC) , 0 );
    printf("Hello message sent\n");    

    // read incoming message
    read(sock, buffer, 1024); // remember: read returns an int, corresponding the number of characters entered
    printf("%s\n", buffer);

    // sleep timer
    cout << "\ntime to disconnect: " << SLEEP_TIME << " seconds";
    cout.flush();
    sleep(SLEEP_TIME);
    cout << endl;

    // server is in listening loop
    send(sock , DISCONNECT_RPC , strlen(DISCONNECT_RPC) , 0 );
    printf("Disconnect message sent");    
    // disconnect message sent

    read(sock, buffer, 1024);
    printf("%s\n", buffer);

    return 0; 
} 

