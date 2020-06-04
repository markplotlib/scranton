#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <iostream>
using namespace std;

class HeadsTailsClient {
private:
   int socket;

   //RPC sends here:
   const char *EXIT_MENU = "rpc=exitmenu;";


public:
   HeadsTailsClient(int socket) {
      this->socket = socket;
   }

   ~HeadsTailsClient() {
      cout << "Gclient2 destructed" << endl;
   }

   int getUserChoice() 
   {
      int userChoice; 
      cin >> userChoice; 
      return userChoice; 
   }

   // display menu options to the user 
   void displayMenuWithinThisGame()
   {
      cout << "\nGame Options\n";
      cout << "\nSelect from the following options\n";
      cout << "1 - make a guess\n";
      cout << "2 - exit this game\n";
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
         displayMenuWithinThisGame(); 
         choice = getUserChoice();
         switch(choice)
         {
            case 1:
            {
               launchHeadsTails(socket);
               break;
            }
            case 2:
            {
               send(socket , EXIT_MENU, strlen(EXIT_MENU) , 0);
               cout << "\nExiting menu\n";
               // placeholder read
               read(socket, buffer, 1024);
               cout << "Has read" << endl;
               return 2;
               break;
            } 
            default:
               break; 
            }
      } while (true);

   return 0;
   }
   

   void launchHeadsTails(int sockNum)
   {
      // client-side display and prompt
      char buff[1024] = {0};
      cout << "\nYou have chosen Extreme Heads or Tails\nGuess the coin flip.  h)eads  t)ails.  Enter any other key to exit: ";
      char guess[2]; // must be an array ending in the null terminator -McKee
      cin >> guess;
      guess[1] = 0;  // 0 is the null terminator
      while (guess[0] == 'h' || guess[0] == 't')
      {
         // build the flipcoin rpc: "rpc=flipcoin;guess=h"  (or "guess=t")
         strcpy(buff, "rpc=flipcoin;");
         strcat(buff, "guess=");
         strcat(buff, guess);
         strcat(buff, ";");

         // sending game selection in buffer
         send(sockNum, buff , strlen(buff), 0 );
         
         // clear the buffer before overwriting it with incoming buffer
         memset(buff, 0, 1024);
         // read message
         read(sockNum, buff, 1024);
         char face[2];
         face[0] = buff[0];
         face[1] = 0;

         string coin = face[0] == 'h' ? "heads" : "tails";
         cout << "The coin shows ___" << coin << "___. " << endl;

         if (guess[0] == face[0])
         {
            cout << "You win! :)" << endl;
         }
         else
         {
            cout << "Sorry :(" << endl;
         }

         cout << "Play again?  h)eads  t)ails.  Enter any other key to exit: ";
         cin >> guess;
      }

         cout << "That was extreme!!!" << endl;
// TODO
         cout << "Rounds played: " << "->htGetNumRounds()";
         cout << ".  Wins:" << "->htGetNumWins()" << endl;


   }

};

