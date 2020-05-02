// Client side C/C++ program to demonstrate Socket programming 
// ports: Peter 12117, Jara 12115, Mark 12104
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#define PORT 12104

int main(int argc, char const *argv[]) 
{ 
    int sock = 0;
    int valread;
    struct sockaddr_in serv_addr; 
    const char *HELLO = "Hello from server"; 
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

    send(sock , HELLO , strlen(HELLO) , 0 );
    printf("Hello message sent\n");    
    // read incoming message
    valread = read(sock, buffer, 1024);
    printf("%s\n%d", buffer, valread);

    // server is in listening loop
    send(sock , DISCONNECT_RPC , strlen(DISCONNECT_RPC) , 0 );
    printf("Disconnect message sent\n");    
    // disconnect message sent
    valread = read(sock, buffer, 1024);

    printf("%s\n%d", buffer, valread);
    return 0; 
} 

