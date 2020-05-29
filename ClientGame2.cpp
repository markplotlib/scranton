#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <iostream>
using namespace std;

class ClientGame2 {
private:
   int socket;

public:
   ClientGame2(int socket) {
      this->socket = socket;
   }

   ~ClientGame2() {}

   int getUserChoice() 
   {
      int userChoice; 
      cin >> userChoice; 
      return userChoice; 
   }

   // display menu options to the user 
   void displayGameMenu() 
   {
      cout << "\nGame Options\n";
      cout << "\nSelect from the following options\n";
      cout << "1 - Disconnect\n";
      cout << "2 - Exit menu\n";
      // TODO: add game menu/ more options 
      cout << "\nUser Selection: ";
   }


   int gameMenu() {
      char buffer[1024] = {0};
      int choice = 0;
      cout << "Entered game" << endl;
      do
      { 
         cout << "Buffer in CG2 = " << buffer << endl;
         displayGameMenu(); 
         choice = getUserChoice(); 
         switch(choice)
         {
            case 1: {
               return 1;
               break;
            }
            case 2: {
               cout << "\nExiting menu\n";
               return 2;
               break;
            } 
            case 3: {
               // you know, whatever.
               break;
            }
            default:
                  break; 
            }
         } while (true);

   return 0;
   }
   
};