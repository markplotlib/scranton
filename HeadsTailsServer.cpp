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

//char winsArr[2];
cout << "context number: " << context.getWins() << endl;
int number = context.getWins();
//cout << "print winsArr" << winsArr << endl;
// winsArr[1] = 0; // null termin8or
//char roundsArr[2];
//cout << "roundsArr = " << roundsArr << endl;
cout << "context number: " << context.getRounds() << endl;
int rounds = context.getRounds();
// roundsArr[1] = 0;
//cout << winsArr << endl;
//cout << roundsArr << endl;
char b[100];
char c[10];
sprintf(b, "%d", number);
char w[7] = " Wins ";
strcat(b, w);
//cout << "Wins: " << b << endl; 
sprintf(c, "%d", rounds);
char r[8] = " Rounds";
strcat(c, r);
strcat(b, c);
//cout << "Rounds: " << c << endl;
send(socket, b, strlen(b) , 0);



            // limited to ~ 10 million rounds to preserve sanity of clients
            //char scores[64] = {0};
            //strcpy(scores, "wins: ");
            //strcat(scores, winsArr);
            //strcat(scores, "; rounds: ");
            //strcat(scores, roundsArr);

           // std::cout << "server about to send scores, line 95" << std::endl;
            //send(socket, scores, strlen(scores) , 0);

            // send(socket , CONFIRMATION, strlen(CONFIRMATION) , 0);
        }

    }
    
    return 0;
}