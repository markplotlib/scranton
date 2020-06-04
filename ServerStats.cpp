#include <iostream>
#include "ServerStats.h"
using namespace std;


ServerStats::ServerStats() { numActiveClients = 0; }

void ServerStats::setHeadsTailsHS(int newScore) {
   pthread_mutex_lock(&highscoreLock);
   if (newScore > headsTailsHS) 
   {
      headsTailsHS = newScore;
      pthread_mutex_unlock(&highscoreLock);
   }
   pthread_mutex_unlock(&highscoreLock);
}

int ServerStats::getHeadsTailsHS() {
   return headsTailsHS;
}

int ServerStats::getLifetimeConnections() {
   return lifetimeConnections;
}

int ServerStats::getNumActiveClients() { return numActiveClients; }
void ServerStats::incrementNumActiveClients() { numActiveClients++; lifetimeConnections++; }
void ServerStats::decrementNumActiveClients() { numActiveClients--; }
