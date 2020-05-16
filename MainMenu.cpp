#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include <iostream>


class MainMenu {
private:
   char buffer[1024] = {0};
   char DISCONNECT_RPC[1024] = "disconnected";

public:
   MainMenu() {
      // debug:
      // std::cout << "MainMenu constructor" << std::endl;
   }

   void loop(int new_socket) {
      bool disconnectFlag = false;
      
      while (!disconnectFlag) {
         read(new_socket, buffer, 1024);
         std::cout << "Buffer reads: " << buffer << "in main menu." << std::endl;
         if (*buffer == *DISCONNECT_RPC) {
            // IF disconnection is successful
            if (disconnect(new_socket, buffer) == 0) {
               memset(buffer, 0, sizeof(buffer));
               disconnectFlag = true;
            }
         } else {
            send(new_socket , buffer, strlen(buffer) , 0 );
            memset(buffer, 0, sizeof(buffer));
         }
      }
      std::cout << "Exit loop in MainMenu" << std::endl;
   }

   // Sends a message to client, and then closes the socket assigned to current client.
   // return 0 if successful 
   // return -1 if failed
   int disconnect(int socket_num, char *buff){
      // char disconnectMsg[1024] = {0};
      send(socket_num, buff, strlen(buff) , 0 );
      // close active socket
      std::cout << "Hello from Main Menu disconnect" << std::endl;
      return close(socket_num);
   }
};