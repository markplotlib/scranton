#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include <iostream>
#include "GameClass2.h"

GameClass2::GameClass2(int socket, ServerStats &serverStats) {
   this->socket = socket;
   this->serverStats = serverStats;
}

GameClass2::~GameClass2() {}

void GameClass2::gameMenu() {

        bool connected = true;
        int readStatus;
        while (connected) {
            memset(buffer, 0, sizeof(buffer));
            readStatus = read(socket, buffer, 1024);
            
            if (readStatus == 0) {
                connected = false;
            }

            // debug code:
            std::cout << "Buffer reads \'" << buffer << "\', in Game2." << std::endl;

            // whatever you want
            if (strcmp(buffer , RPC_1) == 0 ) {

            }
            
            // whatever you want
            else if (strcmp(buffer, RPC_2) == 0 ) {

            }

            // check for disconnect rpc call.
            else if (strcmp(buffer , DISCONNECT_RPC) == 0 ) {
                disconnectGame2(socket, DISCONNECT_RPC);
                connected = false;
            } 
            
            // else you need to return something
            else { 
                send(socket , buffer, strlen(buffer) , 0 );
            }                    
        }
    }

   // Sends a message to client, and then closes the socket assigned to current client.
   // return 0 if successful, -1 if failed
   int GameClass2::disconnectGame2(int socket_num, char *buff) {
      // char disconnectMsg[1024] = {0};
      send(socket_num, buff, strlen(buff) , 0 );
      // close active socket
      return close(socket_num);
   }