#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include <iostream>
#include "HeadsTailsServer.h"
#include "ThreadContext.h"


HeadsTailsServer::HeadsTailsServer(int socket, ServerStats &serverStats)
{
    this->socket = socket;
    this->serverStats = serverStats;
}


HeadsTailsServer::~HeadsTailsServer() { }


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
    while (connected)
    {
        memset(buffer, 0, sizeof(buffer));
        readStatus = read(socket, buffer, 1024);

        if (readStatus == 0)
        {
            // error checking: socket didn't receive message
            connected = false;
        }

        KeyValue rpcKV;
        interpreter.newRPC(buffer);
        interpreter.getNextKeyValue(rpcKV);
        
        //disconnect rpc will skip this check
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

            send(socket, face, 2, 0);
        }

        // Exit menu
        if (strcmp(rpcKV.getValue(), EXIT_MENU) == 0)
        {
            connected = false;

            cout << "context number: " << context.getWins() << endl;
            int number = context.getWins();
            cout << "context number: " << context.getRounds() << endl;
            int rounds = context.getRounds();

            // Add context nunbers to char arrays and add those together
            char winBuffer[100];
            char roundBuffer[10];
            sprintf(winBuffer, "%d", number);
            char winsArr[7] = " Wins ";
            strcat(winBuffer, winsArr);

            sprintf(roundBuffer, "%d", rounds);
            char roundsArr[8] = " Rounds";
            strcat(roundBuffer, roundsArr);
            strcat(winBuffer, roundBuffer);

            //Send the context char array to the client 
            send(socket, winBuffer, strlen(winBuffer) , 0);

        }

    }
    
    return 0;
}