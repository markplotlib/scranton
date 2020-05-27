#include <iostream>
using namespace std;


 
    volatile int numActiveClients;
    // Q: Should this be volatile???  Either stress-test it out, or ask Mike McKee.


    ServerStats::ServerStats() { numActiveClients = 0; }
    // ~ServerStats() { delete numActiveClients; }
        // ^-- I will not need this, unless it's instantiated dynamically (on the heap).
    int ServerStats::getNumActiveClients() { return numActiveClients;}
    void ServerStats::incrementNumActiveClients() { numActiveClients++; }
    void ServerStats::decrementNumActiveClients() { numActiveClients--; }
