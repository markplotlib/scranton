#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include "StringParser.h" 
#include <iostream>
#include "ServerStats.h"

class MainMenu {
private:
    char buffer[1024] = {0};                                // buffer for socket listening
    char DISCONNECT_RPC[1024] = "rpc=disconnect;";
    char SELECTGAME1_RPC[1024] = "rpc=selectgame;game=1;";
    char SELECTGAME2_RPC[1024] = "rpc=selectgame;game=2;";
    char SERVER_STATS_RPC[1024] = "rpc=returnStats;";
    int socket;                                                 // socket to listen from
    StringParser interpreter;
    ServerStats serverStats;

public:
    MainMenu(int socket, ServerStats &serverStats) {
        this->socket = socket;
        this->serverStats = serverStats;
        // std::cout << "MainMenu constructor" << std::endl;
    }
    
    // loop takes a thread and reads/sends until it reads the disconnect RPC. At that point 
    void loop() {

        bool connected = true;

        while (connected) {
            memset(buffer, 0, sizeof(buffer));
            read(socket, buffer, 1024);
            std::cout << "Buffer reads \'" << buffer << "\', in main menu." << std::endl;

            // check for select game rpc 
            if (strcmp(buffer, SERVER_STATS_RPC) == 0 ) {
                memset(buffer, 0, sizeof(buffer));
                int numClients = serverStats.getNumActiveClients();
                sprintf(buffer,"%d", numClients);
                std::cout << "ServerStats RPC accessed" << std::endl;
                send(socket, buffer, strlen(buffer), 0 );
            }
            else if (strcmp(buffer , SELECTGAME2_RPC) == 0) {
                //TODO: RUN GAME 2 HERE 
            }

            // check for disconnect rpc call.
            else if (strcmp(buffer , DISCONNECT_RPC) == 0 ) {
                disconnectMainMenu(socket, DISCONNECT_RPC);
                connected = false;
            } else { 
                send(socket , buffer, strlen(buffer) , 0 );
            }                    
        }
    }

    // Sends a message to client, and then closes the socket assigned to current client.
    // return 0 if successful, -1 if failed
    int disconnectMainMenu(int socket_num, char *buff) {
        // char disconnectMsg[1024] = {0};
        send(socket_num, buff, strlen(buff) , 0 );
        // close active socket
        return close(socket_num);
    }
};
