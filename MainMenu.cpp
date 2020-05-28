#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include "StringParser.h" 
#include <iostream>
#include "ServerStats.h"
using namespace std;

class MainMenu {
private:
    char buffer[1024] = {0};                                // buffer for socket listening
    char DISCONNECT_RPC[1024] = "rpc=disconnect;";

    // temporary
    char FLIPGUESS_H_RPC[1024] = "rpc=flipcoin;guess=h;";
    char FLIPGUESS_T_RPC[1024] = "rpc=flipcoin;guess=t;";

    // char SELECTGAME2_RPC[1024] = "rpc=selectgame;game=2;";
    char SERVER_STATS_RPC[1024] = "rpc=returnStats;";
    int socket;                                                 // socket to listen from
    // int htWins = 0;
    int htRounds = 0;
    StringParser interpreter;
    ServerStats serverStats;

public:
    MainMenu(int socket, ServerStats &serverStats) {
        this->socket = socket;
        this->serverStats = serverStats;
        // std::cout << "MainMenu constructor" << std::endl;
    }


    void buildOutcomeBuffer(char* buff, char guess, string face)
    {
        htRounds++;
        memset(buff, 0, 1024);
        if (guess == face[0])
        {
            // htWins++;
            strcpy(buff, "You've won :)\n");
        }
        else
        {
            strcpy(buff, "Sorry :(\n");    
        }
    }


    string flipCoin() 
    {
        srand (time(NULL));
        string face = rand() % 2 == 0 ? "heads" : "tails";
        return face;
    }

    // TODO: activate this
    // int htGetNumWins() { return htWins; }
    // TODO: activate this

    int htGetNumRounds() { return htRounds; }


    /*
        MAIN MENU LOOP
    */
    // loop takes a thread and reads/sends until it reads the disconnect RPC. At that point 
    void loop() {

        // TEMP CODE TEMP CODE TEMP CODE TODO
        char temphardcode = 'h'; // temphardcode temporary temporarytemporarytemporary!
        // TODO -- have the parser parse it out.
        string winningFace;
        // TEMP CODE TEMP CODE TEMP CODE TODO

        bool connected = true;

        while (connected) {
            memset(buffer, 0, sizeof(buffer));
            read(socket, buffer, 1024);
            std::cout << "Buffer reads \'" << buffer << "\', in MM." << std::endl;

            // Does client's rpc indicate desire to play game?
            // TODO: instead of the complex if clause, it will be simplified via parser
            if ( (strcmp(buffer , FLIPGUESS_H_RPC) == 0 ) || (strcmp(buffer , FLIPGUESS_T_RPC) == 0 ) )
            {
                // initiates Heads or Tails game
                winningFace = flipCoin();
                buildOutcomeBuffer(buffer, temphardcode, winningFace); // temphardcode temporary temporarytemporarytemporary!

                // std::cout << "####inside MM. line ~100.\nbuffer is : " << buffer << std::endl; 

                // sending game selection in buffer
                send(socket, buffer , strlen(buffer) , 0 );
            }            
            
            // server stats selection made by client
            else if (strcmp(buffer, SERVER_STATS_RPC) == 0 )
            {
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer,"%d", serverStats.getNumActiveClients());
                send(socket, buffer, strlen(buffer), 0 );
            }
            
            // disconnect rpc call made by client
            else if (strcmp(buffer , DISCONNECT_RPC) == 0 )
            {
                disconnectMainMenu(socket, DISCONNECT_RPC);
                connected = false;
            }

            else
            { 
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
