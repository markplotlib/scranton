#include <iostream>
#include "ServerStats.h"

// ctor
ServerStats::ServerStats() {
    numActiveClients = 0;
}


// accessors (getters)
int ServerStats::getHeadsTailsHS() { return headsTailsHS; }

int ServerStats::getLifetimeConnections() { return lifetimeConnections; }

int ServerStats::getNumActiveClients() { return numActiveClients; }


// mutators (setters)
void ServerStats::setHeadsTailsHS(int newScore)
{
    pthread_mutex_lock(&highscoreLock);
    
    if (newScore > headsTailsHS) 
    {
        headsTailsHS = newScore;
        pthread_mutex_unlock(&highscoreLock);
    }
    pthread_mutex_unlock(&highscoreLock);
}


void ServerStats::incrementNumActiveClients() {
    numActiveClients++; 
    lifetimeConnections++;
}

void ServerStats::decrementNumActiveClients() { numActiveClients--; }
