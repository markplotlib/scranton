#ifndef SERVER_STATS_H
#define SERVER_STATS_H
#include <pthread.h>

class ServerStats
{

private: 
    volatile int numActiveClients;
    volatile int headsTailsHS;
    volatile int lifetimeConnections;
    pthread_mutex_t highscoreLock;

public:
    ServerStats();
    
    // accessors (getters)
    int getHeadsTailsHS();

    int getLifetimeConnections();
    
    int getNumActiveClients();

    // mutators (setters)
    void setHeadsTailsHS(int newScore);

    void incrementNumActiveClients();

    void decrementNumActiveClients();
};

#endif