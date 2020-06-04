#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include <iostream>
#include "HeadsTails.h"

HeadsTails::HeadsTails(int socket, ServerStats &serverStats) {
    std::cout << "GC2 ctor" << std::endl;
    this->socket = socket;
    this->serverStats = serverStats;
}

HeadsTails::~HeadsTails() { 
    std::cout << "GC2 dtor" << std::endl;
    }


string HeadsTails::flipCoin() 
{
    srand (time(NULL));
    string face = rand() % 2 == 0 ? "h" : "t";
    return face;
}


int HeadsTails::htGetNumRounds() { return htRounds; }


void HeadsTails::updateScoreboard(string guess, string face)
{
    if (guess == face)  // did guess match the flipCoin face?
    {
        // win
        htWins++;
    }
    htRounds++;
}



int HeadsTails::gameMenu() {

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

            // Exit menu
            if (strcmp(buffer , EXIT_MENU) == 0 ) {
                connected = false;
                std::cout << "BUGFIX server about to send" << std::endl;
                send(socket , CONFIRMATION, strlen(CONFIRMATION) , 0);
                std::cout << "BUGFIX server about to send" << std::endl;
            }
            
            // whatever you want
            else if (strcmp(buffer, RPC_2) == 0 ) {

            }
            
            // else you need to return something
            else { 
                return 0;
            }                    
        }
        
        return 0;
    }