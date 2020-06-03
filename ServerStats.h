#ifndef SERVER_STATS_H
#define SERVER_STATS_H
#include <pthread.h>

class ServerStats {

private: 
    volatile int numActiveClients;
    volatile int headsTailsHS;
    pthread_mutex_t highscoreLock;
public:
    ServerStats();
    // ~ServerStats() { delete numActiveClients; }
        // ^-- I will not need this, unless it's instantiated dynamically (on the heap).
    int getNumActiveClients();

    // returns void instead of bool.
    void setHeadsTailsHS(int newScore);

    int getHeadsTailsHS();
    void incrementNumActiveClients();
    void decrementNumActiveClients();
};

#endif