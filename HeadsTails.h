#ifndef HEADSTAILS_H
#define HEADSTAILS_H

#include "ServerStats.h"

class HeadsTails {

private:

   char buffer[1024] = {0};
   int socket;
   ServerStats serverStats;
   char DISCONNECT_RPC[1024] = "rpc=disconnect;";
   char EXIT_MENU[1024] = "rpc=exitmenu;";
   char CONFIRMATION[1024] = "rpc=confirmed;";
   char RPC_1[1024] = "rpc1";
   char RPC_2[1024] = "rpc2";

public:

    explicit HeadsTails(int socket, ServerStats &serverStats);

    HeadsTails();

    ~HeadsTails();

   int gameMenu();

    // Sends a message to client, and then closes the socket assigned to current client.
    // return 0 if successful, -1 if failed
    int disconnectGame2(int socket, char *buff);
};

#endif