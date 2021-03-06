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
#include "HeadsTailsServer.h"
#include "ThreadContext.h"

using namespace std;

class MainMenu
{
private:
    char buffer[1024] = {0};                                // buffer for socket listening
    char DISCONNECT_RPC[1024] = "rpc=disconnect;";
    char SELECT_HT_RPC[1024] = "rpc=selectgame;game=1;";
    char SELECTGAME2_RPC[1024] = "rpc=selectgame;game=2;";
    char SERVER_STATS_RPC[1024] = "rpc=returnStats;";
    int socket;                                                 // socket to listen from


public:

    MainMenu(int socket) {
        this->socket = socket;
    }


    // MAIN MENU LOOP: takes a thread and reads/sends until it reads the disconnect RPC
    void loopThread(ServerStats &serverStats, ThreadContext &context) {

        bool connected = true;
        int readStatus;

        while (connected)
        {
            memset(buffer, 0, sizeof(buffer));

            // Here is where we read all RPCs from client
            readStatus = read(socket, buffer, 1024);

            if (readStatus == 0) {
                connected = false;
            }
            std::cout << "Buffer reads \'" << buffer << "\', in MM." << std::endl;

            // start of game 1: Heads-Tails selection ------------------------------
            if (strcmp(buffer , SELECT_HT_RPC) == 0 )
            {
                send(socket, buffer , strlen(buffer) , 0 );
                memset(buffer, 0, sizeof(buffer));

                cout << "constructor about to be called" << endl;
                HeadsTailsServer *htSession = nullptr;
                htSession = new HeadsTailsServer(socket, serverStats);
                int gameRetVal = htSession->gameMenu(context);

                cout << "destructor about to be called" << endl;
                delete htSession;
                if (gameRetVal == 1)
                {   // 1 is the disconnect code 
                    disconnectMainMenu(socket, DISCONNECT_RPC);
                    connected = false;
                }
            }
            // end of of game 1: Heads-Tails selection ------------------------------


            // Start of game slot 2 selection ------------------------------
            // Application Programmer: your game can be interfaced here.
            if (strcmp(buffer , SELECTGAME2_RPC) == 0 ) 
            {
                send(socket, buffer , strlen(buffer) , 0 );
                memset(buffer, 0, sizeof(buffer));
                GameClass2 *GameClass2Ptr = nullptr;
                GameClass2Ptr = new GameClass2(socket, serverStats);
                int gameRetVal = GameClass2Ptr->gameMenu();
                delete GameClass2Ptr;
                if (gameRetVal == 1) {
                    disconnectMainMenu(socket, DISCONNECT_RPC);
                    connected = false;
                } else {
                    send(socket , buffer, strlen(buffer) , 0 );
                }
            }
            // end of of game slot 2 selection ------------------------------

            // Returns server information
            else if (strcmp(buffer, SERVER_STATS_RPC) == 0 ) 
            {
                
                memset(buffer, 0, sizeof(buffer));
                char lifetimeClientsBuffer[100];
                //char highScoreBuffer[100];
                char lifetime[23] = ": Lifetime Clients || ";
                char clients[21] = ": Active Clients || ";
                // char highScore[17] = ": High Score || ";

                sprintf(lifetimeClientsBuffer,"%d", serverStats.getLifetimeConnections());
                strcat(lifetimeClientsBuffer, lifetime);

                sprintf(buffer,"%d", serverStats.getNumActiveClients());
                strcat(buffer, clients);
                
                // sprintf(highScoreBuffer,"%d", serverStats.getHeadsTailsHS());
                // strcat(highScoreBuffer, highScore);

                strcat(buffer, lifetimeClientsBuffer);
                // strcat(buffer, highScoreBuffer);

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


    // Sends a message to client, and then closes the socket assigned to current client.
    // return 0 if successful, -1 if failed
    int disconnectMainMenu(int socket_num, char *buff) {
        // char disconnectMsg[1024] = {0};
        send(socket_num, buff, strlen(buff) , 0 );
        // close active socket
        return close(socket_num);
    }

};
