#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include "StringParser.h" 
#include <iostream>
#include "HeadsTails.cpp"
using namespace std;

class MainMenu {
private:
    char buffer[1024] = {0};                                // buffer for socket listening
    char DISCONNECT_RPC[1024] = "rpc=disconnect;";
    char SELECTGAME1_RPC[1024] = "rpc=selectgame;game=1;";
    char SELECTGAME2_RPC[1024] = "rpc=selectgame;game=2;";
    int socket;                                                 // socket to listen from
    StringParser interpreter;

public:
    MainMenu(int socket) {
        this->socket = socket;
        // std::cout << "MainMenu constructor" << std::endl;
    }

    void launchHeadsTailsGame() {
        HeadsTails game = HeadsTails();
        printf("printf -- launchHeadsTailsGame FROM MainMenu (aka GameServer).\n");
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "<<<<<<<<< DOES ANYONE READ THIS??? >>>>>>>>> BUFFER from MainMenu (aka GameServer).\n");
        // strcat(buffer, "launchHeadsTailsGame FROM MainMenu (aka GameServer).\n");
        send(socket, buffer, strlen(buffer), 0);
        game.addNum();
        // printf(game.getDog());
// cout << "You've won " << game.getNumWins() << " out of " << game.getNumRounds() << " rounds" << endl;
// cout << "Well, this was lots of fun. Goodbye." << endl;
    }

    
    // loop takes a thread and reads/sends until it reads the disconnect RPC. At that point 
    void loop() {

        bool connected = true;

        while (connected) {
            memset(buffer, 0, sizeof(buffer));
            read(socket, buffer, 1024);
            std::cout << "Buffer reads \'" << buffer << "\', in main menu." << std::endl;

            // check for select game rpc 
            if (strcmp(buffer , SELECTGAME1_RPC) == 0 ) {
                std::cout << "TEST TEST in main menu." << std::endl;
                launchHeadsTailsGame();
            }
            else if (strcmp(buffer , SELECTGAME2_RPC) == 0) {
                //TODO: RUN GAME 2 HERE 
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
