#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include <iostream>
#include "StringParser.h" 
#include "ServerStats.h"
#include "GameClass2.h"

using namespace std; // TEMP. TODO, remove this!!

class MainMenu {
private:
    char buffer[1024] = {0};                                // buffer for socket listening
    char DISCONNECT_RPC[1024] = "rpc=disconnect;";

    //  temporary -- TODO: parse this out.
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
        char f[2]; // must be an array ending in the null terminator -McKee
        f[0] = face[0];
        f[1] = 0; // 0 is the null terminator
        htRounds++;
        strcpy(buff, "face=");
        strcat(buff, f);
        if (guess == f[0])
        {
            // win
            // htWins++;
            strcat(buff, ";outcome=w;");
        }
        else
        {
            // loss
            strcat(buff, ";outcome=l;");
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
        int readStatus;

        while (connected) {
            memset(buffer, 0, sizeof(buffer));
            readStatus = read(socket, buffer, 1024);
            if (readStatus == 0) {
                connected = false;
            }
            std::cout << "Buffer reads \'" << buffer << "\', in MM." << std::endl;

            // Does client's rpc indicate desire to play game?
            // TODO: instead of the complex if clause, it will be simplified via parser
            if ( (strcmp(buffer , FLIPGUESS_H_RPC) == 0 ) || (strcmp(buffer , FLIPGUESS_T_RPC) == 0 ) )
            {
                std::cout << "####inside MM. before line ~100.\nbuffer is : " << buffer << std::endl; 
                memset(buffer, 0, sizeof(buffer));

                // initiates Heads or Tails game
                winningFace = flipCoin();
                std::cout << "####inside MM. before buildOutcomeBuffer.\nbuffer is : " << buffer << std::endl; 
                buildOutcomeBuffer(buffer, temphardcode, winningFace); // temphardcode temporary temporarytemporarytemporary!
                std::cout << "####inside MM.  after buildOutcomeBuffer.\nbuffer is : " << buffer << std::endl; 

                // std::cout << "####inside MM. line ~100.\nbuffer is : " << buffer << std::endl; 

                // sending game selection in buffer
                send(socket, buffer , strlen(buffer) , 0 );
            }            
            
            if (strcmp(buffer , SELECTGAME2_RPC) == 0 ) 
            {
                GameClass2 *gameClass2Ptr = nullptr;
                gameClass2Ptr = new GameClass2(socket, serverStats);
                gameClass2Ptr->gameMenu();
                delete gameClass2Ptr;
            }


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
