#include <iostream>
#include <unistd.h> 
using namespace std;

void printTitle();

int main() {
    int wins = 0, rounds = 0;
    cout << "\nGet ready for the video game sensation that's been sweeping the nation..." << endl;
    sleep(2);
    printTitle();

    char in = '_';  // default
    while (in != 'x') {
        cout << "Menu options: h) Guess 'heads'. t) Guess 'tails'. x) Exit." << endl;
        cout << "Please enter an option: ";
        cin >> in;
        if (in == 'h' || in == 't') {
            srand (time(NULL));
            string coin = rand() % 2 == 0 ? "heads" : "tails";
            cout << "The coin shows <<" << coin << ">>. ";
            if (in == coin[0]) {
                wins++;
                cout << "Well, whoop dee doo. You got it right.\nPlay again?" << endl;
            } else
                cout << "You must practice, and concentrate. A winner keeps the eyes on the prize.\nPlay again?" << endl;
            rounds++;
        }
    }

    cout << "You've won " << wins << " out of " << rounds << " rounds" << endl;
    cout << "Well, this was lots of fun. Goodbye." << endl;
    return 0;
}

void printTitle() {
    printf(" _____  __    __  _____   _____    _____       ___  ___   _____ \n");
    printf("| ____| \\ \\  / / |_   _| |  _  \\  | ____|     /   |/   | | ____|\n");
    printf("| |__    \\ \\/ /    | |   | |_| |  | |__      / /|   /| | | |__  \n");
    printf("|  __|    }  {     | |   |  _  /  |  __|    / / |__/ | | |  __| \n");
    printf("| |___   / /\\ \\    | |   | | \\ \\  | |___   / /       | | | |___ \n");
    printf("|_____| /_/  \\_\\   |_|   |_|  \\_\\ |_____| /_/        |_| |_____|\n");
    sleep(1);
    printf("\t _   _   _____       ___   _____   _____  \n");
    printf("\t| | | | | ____|     /   | |  _  \\ /  ___/ \n");
    printf("\t| |_| | | |__      / /| | | | | | | |___  \n");
    printf("\t|  _  | |  __|    / / | | | | | | \\___  \\ \n");
    printf("\t| | | | | |___   / /  | | | |_| |  ___| | \n");
    printf("\t|_| |_| |_____| /_/   |_| |_____/ /_____/ \n");
    sleep(1);
    printf("\t\t\t _____   _____   \n");
    printf("\t\t\t/  _  \\ |  _  \\  \n");
    printf("\t\t\t| | | | | |_| |  \n");
    printf("\t\t\t| | | | |  _  /  \n");
    printf("\t\t\t| |_| | | | \\ \\  \n");
    printf("\t\t\t\\_____/ |_|  \\_\\ \n");
    sleep(1);
    printf("\t _____       ___   _   _       _____  \n");
    printf("\t|_   _|     /   | | | | |     /  ___/ \n");
    printf("\t  | |      / /| | | | | |     | |___  \n");
    printf("\t  | |     / / | | | | | |     \\___  \\ \n");
    printf("\t  | |    / /  | | | | | |___   ___| | \n");
    printf("\t  |_|   /_/   |_| |_| |_____| /_____/ \n\n");
    sleep(1);
}
