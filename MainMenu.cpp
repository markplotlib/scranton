#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include <iostream>


class MainMenu {
private:
   char buffer[1024] = {0};                        // buffer for socket listening
   char DISCONNECT_RPC[1024] = "disconnected";     // used with disconnect RPC, delete if not in use
   int socket;                                     // socket to listen from

public:
   MainMenu(int socket) {
      this->socket = socket;
      // std::cout << "MainMenu constructor" << std::endl;
   }

   // loop takes a thread and reads/sends until it reads the disconnect RPC. At that point 
   void loop() {

      bool connected = true;

      while (connected) {
         memset(buffer, 0, sizeof(buffer));
         read(socket, buffer, 1024);
         
         // std::cout << "Buffer reads \'" << buffer << "\', in main menu." << std::endl;
         if (*buffer == *DISCONNECT_RPC) {
            disconnectMM(socket, DISCONNECT_RPC);
            // std::cout << "Exiting..." << std::endl;
            connected = false;
         } else {
            send(socket , buffer, strlen(buffer) , 0 );
            memset(buffer, 0, sizeof(buffer));
         }
      }
   }

   // Sends a message to client, and then closes the socket assigned to current client.
   // return 0 if successful, -1 if failed
   int disconnectMM(int socket_num, char *buff) {
      // char disconnectMsg[1024] = {0};
      send(socket_num, buff, strlen(buff) , 0 );
      // close active socket
      return close(socket_num);
   }

};