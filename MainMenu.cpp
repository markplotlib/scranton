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
    // char SELECTGAME1_RPC[1024] = "rpc=selectgame;game=1;";
    // char SELECTGAME2_RPC[1024] = "rpc=selectgame;game=2;";
    char SAMPLE_RPC[1024] = "opengame";
    char SELECTGAME1_RPC[1024] = "rpc=selectgame;game=1";
    char SELECTGAME2_RPC[1024] = "rpc=selectgame;game=2";
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

    void flipCoin() {
        memset(buffer, 0, sizeof(buffer));
        std::cout << "#### in MainMenu.flipCoin(), around line 35.\nBuffer reads \'" << buffer << "\', ." << std::endl;
        send(socket , buffer, strlen(buffer) , 0 );  // sends to 
        char guess;
        cin >> guess;
        if (guess == 'h' || guess == 't') {
            srand (time(NULL));
            string coin = rand() % 2 == 0 ? "heads" : "tails";
            cout << "The coin shows ___" << coin << "___. ";

            if (guess == coin[0]) {
                // htWins++;
                cout << "You've won :)\nPlay again?" << endl;
            } else
                cout << "Sorry :(\nPlay again?" << endl;
            htRounds++;
        }
        // cout << "You've won " << htGetNumWins() << " out of " << htGetNumRounds() << " rounds" << endl;
        cout << "You've played " << htGetNumRounds() << " rounds" << endl;
        cout << "Well, this was lots of fun. Goodbye." << endl;
    }

    // int htGetNumWins() { return htWins; }
    int htGetNumRounds() { return htRounds; }
    
    // loop takes a thread and reads/sends until it reads the disconnect RPC. At that point 
    void loop() {

        bool connected = true;
        int readStatus;

        while (connected) {
            memset(buffer, 0, sizeof(buffer));
            readStatus = read(socket, buffer, 1024);
            if (readStatus == 0) {
                connected = false;
            }
            std::cout << "Buffer reads \'" << buffer << "\', in MM." << std::endl;

            // check for select game rpc 
            if (strcmp(buffer , SELECTGAME1_RPC) == 0 ) {
                // initiates Heads or Tails game
                flipCoin();
                // disconnect
                disconnectMainMenu(socket, DISCONNECT_RPC);
                connected = false;
            }
            
            if (strcmp(buffer , SELECTGAME2_RPC) == 0 ) {
                GameClass2 *gameClass2Ptr = nullptr;
                gameClass2Ptr = new GameClass2(socket, serverStats);
                gameClass2Ptr->gameMenu();
                delete gameClass2Ptr;
            }


            else if (strcmp(buffer, SERVER_STATS_RPC) == 0 ) {
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer,"%d", serverStats.getNumActiveClients());
                send(socket, buffer, strlen(buffer), 0 );
            }

            // check for disconnect rpc call.
            else if (strcmp(buffer , DISCONNECT_RPC) == 0 ) {
                disconnectMainMenu(socket, DISCONNECT_RPC);
                connected = false;
            } else { 
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
