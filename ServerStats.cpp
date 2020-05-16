#include <iostream>
using namespace std;

// Q: Should this be a singleton instead?
// Q: Should this be a struct instead?

class ServerStats {
private: 
    volatile int numActiveClients;
    // Q: Should this be volatile???  Either stress-test it out, or ask Mike McKee.

public:
    ServerStats() { numActiveClients = 0; }
    // ~ServerStats() { delete numActiveClients; }
        // ^-- I will not need this, unless it's instantiated dynamically (on the heap).
    int getNumActiveClients() { return numActiveClients;}
    void incrementNumActiveClients() { numActiveClients++; }
    void decrementNumActiveClients() { numActiveClients--; }
};


// To test this out, in a separate, temporary driver file:

int main() {
    int num;
    cout << "Accessing ServerStats." << endl;
    ServerStats stats = ServerStats();
    cout << "# active clients: " << stats.getNumActiveClients() << endl;;

    cout << "And then 5 clients connect..." << endl;
    stats.incrementNumActiveClients(); stats.incrementNumActiveClients(); stats.incrementNumActiveClients(); stats.incrementNumActiveClients(); stats.incrementNumActiveClients();
    cout << "# active clients: " << stats.getNumActiveClients() << endl;;

    cout << "And then 3 clients disconnect..." << endl;
    stats.decrementNumActiveClients(); stats.decrementNumActiveClients(); stats.decrementNumActiveClients();
    cout << "# active clients: " << stats.getNumActiveClients() << endl;;

    cout << "Exiting ServerStats." << endl;
    return 0;
}

