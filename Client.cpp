// Client side C/C++ program to demonstrate Socket programming 
// ports: Peter 12117, Jara 12115, Mark 12104
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <iostream>
#include "ClientGame2.cpp"
#include "HeadsTailsClient.cpp"
#define PORT 12104
using namespace std;

// Get user choice for menu switch
int getUserChoice() 
{
    int userChoice; 
    cin >> userChoice; 
    return userChoice; 
}

// Display menu options to the user 
void displayUserMenu() 
{
    cout << "\nMenu Options\n";
    cout << "\nSelect from the following options\n";
    cout << "1 - Disconnect\n";
    cout << "2 - Game Menu\n";
    cout << "3 - Server stats\n";
    // TODO: add game menu/ more options 
    cout << "\nUser Selection: ";
}

// Display game menu options to the user
void displayGameMenu() 
{
    cout << "\nGame Menu Options\n";
    cout << "\nSelect from the following options\n";
    cout << "1 - Extreme Heads or Tails\n";
    cout << "2 - GAME2\n";
    cout << "3 - Back to Main Menu\n";
    cout << "\nUser Selection: ";
}

// Add the user arguments to the character array buffer for connect rpc
void login(char* buffer, char* username, char* password)
{
    strcpy(buffer, "rpc=connect;");  // take note: this is strcpy, not strcat! (easy to overlook)
    strcat(buffer, "user=");
    strcat(buffer, username);
    strcat(buffer, ";password=");
    strcat(buffer, password);
    strcat(buffer, ";");
    // puts(buffer);  // another way to print to screen
}

// Get credentials from the user
void getUserCredentials(char* username, char* password)
{
    cout << "\nPlease enter username ('mike'): ";
    cin >> username;
    cout << "Please enter password ('123'): ";
    cin >> password;
    cout << "\nYou've typed " << username << " and " << password << endl;
}

// Create buffer for selectGame RPC
// buffer contains instructions for server to interpret number of client's selection choice
void selectGame(char* buffer, int gameNumber)
{
    // rpc format     "rpc=selectgame;game=1;"
    // add select rpc info to the buffer 
    strcpy(buffer, "rpc=selectgame;");
    strcat(buffer, "game=");

    if(gameNumber == 1)
    {
        strcat(buffer, "1");
    } 
    else if(gameNumber == 2)
    {
        strcat(buffer, "2");
    } 
    strcat(buffer, ";");
}

// Initiates user menu loop for menu and game selections
// Loop is exited by: bad login, or user selecting exit.
int userMenuLoop(int sock, int choice, char buffer[1024], const char *SERVER_STATS_RPC, const char *DISCONNECT_RPC){
// Client start menu loop
  
    do 
    { 
        displayUserMenu(); 
        choice = getUserChoice(); 
        switch(choice)
        {
            case 1: 
                cout << "\nDisconnecting from the Server\n"; 
                // Send choice to ServerGateway to disconnect
                send(sock , DISCONNECT_RPC , strlen(DISCONNECT_RPC) , 0 );
                printf("Disconnect message sent\n");    
                read(sock, buffer, 1024);
                break;
            case 2: 
                cout << "\nOpening Game Menu\n"; 
                // Game Menu Displayed Here 
                do
                {
                    displayGameMenu(); 
                    choice = getUserChoice(); 
                    switch(choice)
                    {
                        case 1: {
                            cout << "\nYou have chosen Extreme Heads or Tails!\n";
                            // Clear the buffer just in case 
                            memset(buffer, 0, 1024);

                            selectGame(buffer, 1); 
                            // send game selection to ServerGateway
                            send(sock , buffer , strlen(buffer) , 0 );


                            // receive buffer from MainMenu
                            read(sock, buffer, 1024);
                            
                            // start up HeadsTails object
                            HeadsTailsClient *HeadsTailsClientPtr;
                            HeadsTailsClientPtr = new HeadsTailsClient(sock);
                            // this is where we interact with Heads Tails game
                            HeadsTailsClientPtr->gameMenu();
                            // session has ended
                            delete HeadsTailsClientPtr;
                            break; 
                        }
                        case 2: {
                            cout << "\nYou have chosen the Legendary Game... II!\n";
                            // clear the buffer just in case 
                            memset(buffer, 0, 1024);
                            // TODO: Game 2 is not implemented yet 
                            selectGame(buffer, 2); 

                            send(sock , buffer , strlen(buffer) , 0 );
                            read(sock, buffer, 1024);
                            ClientGame2 *clientGame2Ptr;
                            clientGame2Ptr = new ClientGame2(sock);
                            // cout << "Entering game" << endl;
                            int game2RetVal = clientGame2Ptr->gameMenu();
                            // cout << "Exited game" << endl;
                            delete clientGame2Ptr;
                            if (game2RetVal == 1) 
                            {
                                cout << "\nDisconnecting from the Server\n"; 
                                // Send choice to server to disconnect
                                send(sock , DISCONNECT_RPC , strlen(DISCONNECT_RPC) , 0 );
                                printf("Disconnect message sent\n");    
                                read(sock, buffer, 1024);
                                return 0;      
                            }
                            break; 
                        }
                        case 3: {
                            cout << "\nSending you back to Main Menu\n";
                            break;
                            }
                    }

                } while (choice != 3); 
                break;
            case 3: 
                send(sock , SERVER_STATS_RPC, strlen(SERVER_STATS_RPC) , 0 );

                // global serverstats context sent in from MainMenu LoopThread
                memset(buffer, 0, 1024);
                read(sock, buffer, 1024);
                cout << "========================================================\n" <<
                        buffer << "\n" <<
                        "========================================================" << endl;
                break;
            default:
                break; 
        }
    } while (choice != 1);
    return 0; 
}

// Runs the client program
int main(int argc, char** argv)
{
    int choice = 0; // to store user choice 
    int sock = 0;   // socket id for client-server binding
    struct sockaddr_in serv_addr; 
    char buffer[1024] = {0}; 
    char argv1[40];
    char argv2[40]; 

    // Get credentials from the user 
    getUserCredentials(argv1, argv2);

    // User login for buffer
    login(buffer, argv1, argv2); 

    const char *SERVER_STATS_RPC = "rpc=returnStats;";
    const char *DISCONNECT_RPC = "rpc=disconnect;"; 

    // creates socket on client side
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 

    // pass struct attributes in: port number,  
    serv_addr.sin_family = AF_INET;  // AF_INET is IPv4 protocol (AF_INET6 is IPv6)
    serv_addr.sin_port = htons(PORT);  // convert IP port number, from host byte order, to network byte order
    
    // Convert IPv4 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 

    // connects the socket (referred to by the file descriptor sockfd)
    // to the address specified by addr. (Server’s address and port is specified in addr).
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }

    // sending username/password rpc to ServerGateway
    send(sock , buffer , strlen(buffer) , 0 );
    // resetting the buffer
    memset(buffer, 0, 1024);
    // receiving buffer sent from server
    read(sock, buffer, 1024); // remember: read returns an int, corresponding the number of characters entered

    // disconnect RPC sent due to bad login credentials
    if (strcmp(buffer, DISCONNECT_RPC) == 0)
    {
        // server is in listening loop
        cout << "\nWrong credentials, disconnecting\n"; 
        return 0;
    }
    
    // user enters while menu loop 
    userMenuLoop(sock, choice, buffer, SERVER_STATS_RPC, DISCONNECT_RPC);
   
    return 0; 
} 
