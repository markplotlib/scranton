#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include <iostream>
#include "HeadsTails.h"

HeadsTails::HeadsTails(int socket, ServerStats &serverStats) {
    std::cout << "HT ctor" << std::endl;
    this->socket = socket;
    this->serverStats = serverStats;
}

HeadsTails::~HeadsTails() { 
    std::cout << "HT dtor" << std::endl;
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


int HeadsTails::gameMenu()
{
    bool connected = true;
    int readStatus;
    while (connected) {
        memset(buffer, 0, sizeof(buffer));
        readStatus = read(socket, buffer, 1024);

        if (readStatus == 0) {
            // error checking: socket didn't receive message
            connected = false;
        }

        KeyValue rpcKV;
        interpreter.newRPC(buffer);
        interpreter.getNextKeyValue(rpcKV);

std::cout << "HeadsTails.cpp, LINE 60. Buffer reads \'" << buffer << "\'." << std::endl;
        //disconnect rpc will skip this check
        if ((strcmp(rpcKV.getValue(), "flipcoin") == 0))
        {   
            KeyValue guessKV;   // guess= h/t
            interpreter.getNextKeyValue(guessKV); // guess= h/t
            string guess = guessKV.getValue();
            if (guess == "h" || guess == "t")
            {
                string winningFace = flipCoin();

                updateScoreboard(guess, winningFace);
            }
        }

        // debug code:
        std::cout << "HeadsTails.cpp, LINE 76. Buffer reads \'" << buffer << "\'." << std::endl;

        // Exit menu
        if (strcmp(rpcKV.getValue(), EXIT_MENU) == 0)
        {
            connected = false;
            std::cout << "BUGFIX server about to send" << std::endl;
            send(socket , CONFIRMATION, strlen(CONFIRMATION) , 0);
            std::cout << "BUGFIX server about to send" << std::endl;
        }
        
        // whatever you want
    // char FLIPGUESS_H_RPC[1024] = "rpc=flipcoin;guess=h;";
    // char FLIPGUESS_T_RPC[1024] = "rpc=flipcoin;guess=t;";
        
        // else you need to return something
        else { 
            return 0;
        }                    
    }
    
    return 0;
}