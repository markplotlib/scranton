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
      const char *DISCONNECT_RPC = "rpc=disconnect;";

      do
      { 
         // clear screen goes here
         displayGameMenu(); 
         choice = getUserChoice(); 
         switch(choice)
         {
            case 1: 
               cout << "\nDisconnecting from the Server\n"; 
               // Send choice to server to disconnect
               send(socket , DISCONNECT_RPC , strlen(DISCONNECT_RPC) , 0 );
               printf("Disconnect message sent\n");  
               read(socket, buffer, 1024);
               // TODO
               return 1;
               break;
            case 2: 
               cout << "\nExiting menu\n";
               return 2;
               break; 
            case 3: 

               break;

            default:
                  break; 
         }
      } while (choice != 1 && choice != 2);
   return 0;
   }
   
};