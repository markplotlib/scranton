#include <iostream>
#include "ServerStats.h"
using namespace std;

ServerStats::ServerStats() { numActiveClients = 0; }
// ~ServerStats() { delete numActiveClients; }
// ^-- I will not need this, unless it's instantiated dynamically (on the heap).
int ServerStats::getNumActiveClients() { return numActiveClients;}
void ServerStats::incrementNumActiveClients() { numActiveClients++; }
void ServerStats::decrementNumActiveClients() { numActiveClients--; }
