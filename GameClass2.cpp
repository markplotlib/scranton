#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include <iostream>
#include "GameClass2.h"

GameClass2::GameClass2(int socket, ServerStats &serverStats) {
    std::cout << "GC2 ctor" << std::endl;
    this->socket = socket;
    this->serverStats = serverStats;
}

GameClass2::~GameClass2() { 
    std::cout << "GC2 dtor" << std::endl;
    }

int GameClass2::gameMenu() {

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
                return 1;
            } 
            
            // else you need to return something
            else { 
                return 0;
            }                    
        }
        
        return 0;
    }