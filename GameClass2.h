#ifndef GAMECLASS2_H
#define GAMECLASS2_H

#include "ServerStats.h"

class GameClass2 {

private:

   char buffer[1024] = {0};
   int socket;
   ServerStats serverStats;
   char DISCONNECT_RPC[1024] = "rpc=disconnect;";
   char RPC_1[1024] = "rpc1";
   char RPC_2[1024] = "rpc2";

public:

    explicit GameClass2(int socket, ServerStats &serverStats);

    GameClass2();

    ~GameClass2();

   void gameMenu();

    // Sends a message to client, and then closes the socket assigned to current client.
    // return 0 if successful, -1 if failed
    int disconnectGame2(int socket, char *buff);
};

#endif