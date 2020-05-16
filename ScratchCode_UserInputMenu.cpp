#include <iostream>
using namespace std;

int main() {
    cout << "executing [user menu] app." << endl;

    char in = '_';  // default
    while (in != 'd') {
        cout << "Menu options: a) Reveal answer. c) Connect. d) Disconnect." << endl;
        cout << "Please enter an option: ";
        cin >> in;
        if (in == 'a') {
            cout << endl << "Answer: " << 42 << endl;
        } else if (in == 'c') {
            cout << endl << "Connecting... " << endl;
        }
    }

    cout << endl << "Disconnecting... " << endl;
    cout << "exiting [user menu] app." << endl;
    return 0;
}