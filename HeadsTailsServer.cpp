#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include <iostream>
#include "HeadsTailsServer.h"
#include "ThreadContext.h"

HeadsTailsServer::HeadsTailsServer(int socket, ServerStats &serverStats) {
    std::cout << "HT ctor" << std::endl;
    this->socket = socket;
    this->serverStats = serverStats;
}

HeadsTailsServer::~HeadsTailsServer() { 
    std::cout << "HT dtor" << std::endl;
}

string HeadsTailsServer::flipCoin() 
{
    srand (time(NULL));
    string face = rand() % 2 == 0 ? "h" : "t";
    return face;
}


void HeadsTailsServer::updateScoreboard(string guess, string face, ThreadContext &context)
{
    if (guess == face)  // did guess match the flipCoin face?
    {
        // win
        context.incrementWins();
    }
    context.incrementRounds();
}


int HeadsTailsServer::gameMenu(ThreadContext &context)
{
    bool connected = true;
    int readStatus;
    while (connected) {
        cout << "====================== entering Heads Tails Server ======================" << endl;


        memset(buffer, 0, sizeof(buffer));
        readStatus = read(socket, buffer, 1024);

        if (readStatus == 0) {
            // error checking: socket didn't receive message
            connected = false;
        }

        KeyValue rpcKV;
        interpreter.newRPC(buffer);
        interpreter.getNextKeyValue(rpcKV);
        
std::cout << "HeadsTailsServer.cpp, LINE 60. Buffer reads \'" << buffer << "\'." << std::endl;
        //disconnect rpc will skip this check
cout << "=========== rpcKV.getValue() =================================";
cout << rpcKV.getValue() << endl;
        if ((strcmp(rpcKV.getValue(), "flipcoin") == 0))
        {   
            string winningFace;
            KeyValue guessKV;   // guess= h/t
            interpreter.getNextKeyValue(guessKV); // guess= h/t
            string guess = guessKV.getValue();
            if (guess == "h" || guess == "t")
            {
                winningFace = flipCoin();

                updateScoreboard(guess, winningFace, context);
            }
            char face[2];
            face[0] = winningFace[0];
            face[1] = 0;

cout << "============================================" << endl;
std::cout << "HeadsTailsServer.cpp, LINE 77. face reads \'" << face << "\'." << std::endl;
            send(socket, face, 2, 0);
        }



        // debug code:
        std::cout << "HeadsTailsServer.cpp, LINE 76. Buffer reads \'" << buffer << "\'." << std::endl;

        // Exit menu
        if (strcmp(rpcKV.getValue(), EXIT_MENU) == 0)
        {
            connected = false;
            std::cout << "BUGFIX server about to send" << std::endl;
            send(socket , CONFIRMATION, strlen(CONFIRMATION) , 0);
            std::cout << "BUGFIX server about to send" << std::endl;
        }

    }
    
    return 0;
}