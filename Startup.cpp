// #include "ServerGateway.h"
// #include "MainMenu.h"
// #include "StringParser.h"
// #include "KeyValue.h"
#include <iostream>
using namespace std;

volatile int numActiveClients = 0;

// class Startup {

    int main() {
        cout << "executing Startup app." << endl;

        // Creates ServerGateway,
            // waiting on this: "creation of ServerGateway.h"
                // once that's done:
                    // instantiate a ServerGateway object

        // create MainMenu, 
            // waiting on this: "creation of MainMenu.h" 
                // once that's done:
                    // instantiate a MainMenu object

        // waiting on card: "Create new files from copied code"
            // once that's done:
                // instantiate a StringParser object, 
                // instantiate a KeyValue object. 

        // waiting on card: "Create new files from copied code"
            // create StringParser/KeyValue as arguments.
                // Q: unclear what method exactly.
                // my assumption: the only method in Startup.cpp is main()
                // it'll become clear whether or not to do this

        char in = '_';  // default
        while (in != 'x') {
            cout << "Menu options: a) Show active user count.  x) Exit." << endl;
            cout << "Please enter an option: ";
            cin >> in;
            if (in == 'a') {
                cout << endl << "Active users: " << numActiveClients << endl;
            }
        }

        cout << "exiting Startup app." << endl;
        return 0;
    }

// };