#include <iostream>
#include "ServerStats.h"
using namespace std;


ServerStats::ServerStats() { numActiveClients = 0; }
// ~ServerStats() { delete numActiveClients; }
// ^-- I will not need this, unless it's instantiated dynamically (on the heap).

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


int ServerStats::getNumActiveClients() { return numActiveClients; }
void ServerStats::incrementNumActiveClients() { numActiveClients++; }
void ServerStats::decrementNumActiveClients() { numActiveClients--; }
