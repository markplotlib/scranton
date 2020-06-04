#ifndef HEADSTAILS_H
#define HEADSTAILS_H

#include "ServerStats.h"
#include "StringParser.h"
#include "ThreadContext.h"
#include <string>
using namespace std;

class HeadsTailsServer {

private:

    char buffer[1024] = {0};
    int socket;
    ServerStats serverStats;
    char DISCONNECT_RPC[1024] = "rpc=disconnect;";
    char EXIT_MENU[1024] = "exitmenu";
    // char CONFIRMATION[1024] = "rpc=confirmed;";
    char RPC_1[1024] = "rpc1";
    char RPC_2[1024] = "rpc2";

    StringParser interpreter;

public:

    explicit HeadsTailsServer(int socket, ServerStats &serverStats);

    HeadsTailsServer();

    ~HeadsTailsServer();

    string flipCoin();

    void updateScoreboard(string, string, ThreadContext &context);

    void getUserGuess();

    int gameMenu(ThreadContext &context);

    // Sends a message to client, and then closes the socket assigned to current client.
    // return 0 if successful, -1 if failed
    int disconnectGame2(int socket, char *buff);
};

#endif