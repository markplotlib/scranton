#ifndef SERVER_STATS_H
#define SERVER_STATS_H
#include <pthread.h>

class ServerStats {

private: 
    volatile int numActiveClients;
    volatile int headsTailsHS;
    volatile int lifetimeConnections;
    pthread_mutex_t highscoreLock;
public:
    ServerStats();

    int getNumActiveClients();

    // returns void instead of bool.
    void setHeadsTailsHS(int newScore);

    int getLifetimeConnections();
    
    int getHeadsTailsHS();
    void incrementNumActiveClients();
    void decrementNumActiveClients();
};

#endif