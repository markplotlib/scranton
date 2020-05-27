#ifndef SERVER_STATS_H
#define SERVER_STATS_H

class ServerStats {

private: 
    volatile int numActiveClients;

public:
    ServerStats() { numActiveClients = 0; }
    // ~ServerStats() { delete numActiveClients; }
        // ^-- I will not need this, unless it's instantiated dynamically (on the heap).
    int getNumActiveClients() { return numActiveClients;}
    void incrementNumActiveClients() { numActiveClients++; }
    void decrementNumActiveClients() { numActiveClients--; }
};

#endif