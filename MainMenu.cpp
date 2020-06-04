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
#include "HeadsTails.h"
using namespace std;

class MainMenu
{
private:
    char buffer[1024] = {0};                                // buffer for socket listening
    char DISCONNECT_RPC[1024] = "rpc=disconnect;";
    char SELECT_HT_RPC[1024] = "rpc=selectgame;game=HT;";
    char SELECTGAME2_RPC[1024] = "rpc=selectgame;game=2;";

    // char SELECTGAME2_RPC[1024] = "rpc=selectgame;game=2;";
    char SERVER_STATS_RPC[1024] = "rpc=returnStats;";
    int socket;                                                 // socket to listen from


public:

    MainMenu(int socket) {
        this->socket = socket;
    }


    // MAIN MENU LOOP: takes a thread and reads/sends until it reads the disconnect RPC
    void loopThread(ServerStats &serverStats) {

        bool connected = true;
        int readStatus;

        while (connected)
        {
            memset(buffer, 0, sizeof(buffer));

            // Here is where we read the flipcoin guess for the client
            readStatus = read(socket, buffer, 1024);

            if (readStatus == 0) {
                connected = false;
            }
            std::cout << "Buffer reads \'" << buffer << "\', in MM." << std::endl;

            // start HTselection ------------------------------
            if (strcmp(buffer , SELECT_HT_RPC) == 0 )
            {
                // placeholder
                send(socket, buffer , strlen(buffer) , 0 );
                memset(buffer, 0, sizeof(buffer));
                cout << "constructor about to be called" << endl;
                HeadsTails *htSession = nullptr;
                htSession = new HeadsTails(socket, serverStats);
                int gameRetVal = htSession->gameMenu();
                cout << "destructor about to be called" << endl;
                delete htSession;
                if (gameRetVal == 1) {
                    disconnectMainMenu(socket, DISCONNECT_RPC);
                    connected = false;
                } else {
                    send(socket , buffer, strlen(buffer) , 0 );
                }
            }
            // end of HT selection ------------------------------


            // start game 2 selection ------------------------------
            if (strcmp(buffer , SELECTGAME2_RPC) == 0 ) 
            {
                // placeholder
                send(socket, buffer , strlen(buffer) , 0 );
                memset(buffer, 0, sizeof(buffer));
                cout << "constructor about to be called" << endl;
                GameClass2 *GameClass2Ptr = nullptr;
                GameClass2Ptr = new GameClass2(socket, serverStats);
                int gameRetVal = GameClass2Ptr->gameMenu();
                cout << "destructor about to be called" << endl;
                delete GameClass2Ptr;
                if (gameRetVal == 1) {
                    disconnectMainMenu(socket, DISCONNECT_RPC);
                    connected = false;
                } else {
                    send(socket , buffer, strlen(buffer) , 0 );
                }
            }
            // end of game 2 selection ------------------------------

            else if (strcmp(buffer, SERVER_STATS_RPC) == 0 ) 
            {
                
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer,"%d", serverStats.getNumActiveClients());
                send(socket, buffer, strlen(buffer), 0 );
            }
            
            // disconnect rpc call made by client VOLUNTARILY
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

/*

    void buildOutcomeBuffer(char* buff, char guess, string face)
    {
        char f[2]; // must be an array ending in the null terminator -McKee
        f[0] = face[0];
        f[1] = 0; // 0 is the null terminator
        htRounds++;
        strcpy(buff, "face=");
        strcat(buff, f);
        if (guess == f[0])  // did guess match the flipCoin face?
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

*/


    // Sends a message to client, and then closes the socket assigned to current client.
    // return 0 if successful, -1 if failed
    int disconnectMainMenu(int socket_num, char *buff) {
        // char disconnectMsg[1024] = {0};
        send(socket_num, buff, strlen(buff) , 0 );
        // close active socket
        return close(socket_num);
    }

};
