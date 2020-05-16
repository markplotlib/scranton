#include <iostream>
// #include <MainMenu.h>

/*
Extractor::Extractor() : Sequence() {}

Extractor::Extractor(string &str) : Sequence(str) {}

string Extractor::extract() {
   return word + " extracted";
}
*/


class MainMenu {

public:
   MainMenu() {
      std::cout << "tolog" << std::endl;
   }

   int loop() {
      std::cout << "yipes!" << std::endl;
      return 1;
   }
   /*
   char buffer[1024] = {0};
   char DISCONNECT_RPC[1024] = "disconnected";
public:

   MainMenu() {

   }

   // for later:
   // read(new_socket, buffer, 1024);
   // send(new_socket , buffer , strlen(buffer) , 0 );

   void loop(int new_socket) {
      bool disconnectFlag = false;
      while (!disconnectFlag) {
         read(new_socket, buffer, 1024);

         if (*buffer == *DISCONNECT_RPC) {
            // IF disconnection is successful
            if (disconnect(new_socket, buffer) == 0)
               disconnectFlag = true;
         } else {
            // TODO: ELSE the buffered message is sent back to client (placeholder)
            send(new_socket , buffer , strlen(buffer) , 0 );
         }
         memset(buffer, 0, sizeof(buffer));
      }

   }

}
*/
};