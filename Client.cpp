// Client side C/C++ program to demonstrate Socket programming 
// ports: Peter 12117, Jara 12115, Mark 12104
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <iostream>
#include "ClientGame2.cpp"
#define PORT 12104
using namespace std;

// void launchHeadsTails(int, char*); deadcode
void launchHeadsTails(int);

// get user choice for menu switch
int getUserChoice() 
{
    int userChoice; 
    cin >> userChoice; 
    return userChoice; 
}

// display menu options to the user 
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

void displayGameMenu() 
{
    cout << "\nGame Menu Options\n";
    cout << "\nSelect from the following options\n";
    cout << "1 - Extreme Heads or Tails\n";
    cout << "2 - GAME2\n";
    cout << "3 - Back to Main Menu\n";
    // TODO: add games/ more options 
    cout << "\nUser Selection: ";
}

// add the user arguments to the buffer 
void login(char* buffer, char* username, char* password)
{
    strcpy(buffer, "rpc=connect;");  // take note: this is strcpy, not strcat! (easy to overlook)
    strcat(buffer, "user=");
    strcat(buffer, username);
    strcat(buffer, ";password=");
    strcat(buffer, password);
    strcat(buffer, ";");
    puts(buffer);  // another way to print to screen
}

// get credentials from the user 
void getUserCredentials(char* username, char* password)
{
    cout << "\nPlease enter username ('mike'): ";
    cin >> username;
    cout << "Please enter password ('123'): ";
    cin >> password;
    cout << "\nYou've typed " << username << " and " << password << endl;
}

void selectGame(char* buffer, int gameNumber)
{
    // rpc format     "rpc=selectgame;game=1;"
  
    // add select rpc info to the buffer 
    strcpy(buffer, "rpc=selectgame;");
    strcat(buffer, "game=");
// TODO: cast gameNumber as string or char
// char c = (char) gameNumber;
// strcat(buffer, c);
    if(gameNumber == 1)
    {
        strcat(buffer, "1");
    } 
    else if(gameNumber == 2)
    {
        strcat(buffer, "2");
    } 
    // does the send need to be here?
    // Mark: No it does not. login function has no send.
}


int main(int argc, char** argv)
{
    // to store user choice 
    int choice = 0; 
    int sock = 0;
    struct sockaddr_in serv_addr; 
    char buffer[1024] = {0}; 
    char argv1[40];
    char argv2[40]; 

    getUserCredentials(argv1, argv2);

    // user login 
    login(buffer, argv1, argv2); 

    const char *SERVER_STATS_RPC = "rpc=returnStats;";
    const char *DISCONNECT_RPC = "rpc=disconnect;"; // thisfix: DISCONNECT_RPC = "disconnect". NOTE: the discrepancy/ambiguity: "disconnect" is the rpc command. "disconnected" is displayed to user.
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    
    // Convert IPv4 and IPv6 addresses from text to binary form 
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

    // sending username/password rpc
    send(sock , buffer , strlen(buffer) , 0 );

    // read incoming message
    memset(buffer, 0, sizeof(buffer));
    read(sock, buffer, 1024); // remember: read returns an int, corresponding the number of characters entered

    if (strcmp(buffer, DISCONNECT_RPC) == 0)
    {
        // server is in listening loop
        cout << "\nWrong credentials, disconnecting\n"; 
        return 0;
    }
    
    // Client start menu loop
    do 
    { 
        // clear screen goes here
        displayUserMenu(); 
        choice = getUserChoice(); 
        switch(choice)
        {
            case 1: 
                cout << "\nDisconnecting from the Server\n"; 
                // Send choice to server to disconnect
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
                            // launchHeadsTails(sock, buffer); deadcode
                            launchHeadsTails(sock);
                            break;
                            }
            
                        case 2: {
                            cout << "\nYou have chosen the Legendary Game... II!\n";
                            // clear the buffer just in case 
                            memset(buffer, 0, sizeof(buffer));
                            // TODO: Game 2 is not implemented yet 
                            selectGame(buffer, 2); 
                            cout << "####Buffer is : " << buffer << endl; 
                            send(sock , buffer , strlen(buffer) , 0 );
                            read(sock, buffer, 1024);
                            ClientGame2 *clientGame2Ptr;
                            clientGame2Ptr = new ClientGame2(sock);
                            int game2RetVal = clientGame2Ptr->gameMenu();
                        
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
                memset(buffer, 0, sizeof(buffer));
                read(sock, buffer, 1024);
                cout << "============================\n" <<
                        "Server has " << buffer << " clients connected.\n" <<
                        "============================" << endl;
                break;

            default:
                break; 
        }

    } while (choice != 1);

    return 0; 
} 


void launchHeadsTails(int sockNum)
{
// char pause; // THIS IS TEMPORARY TEMPORARY TEMPORARY TEMPORARY LINE

    // client-side display and prompt
    char buff[1024] = {0};
    cout << "\nYou have chosen Extreme Heads or Tails\nGuess the coin flip.  h)eads  t)ails.  Enter any other key to exit: ";
    char guess[2]; // must be an array ending in the null terminator -McKee
    cin >> guess;
    guess[1] = 0;  // 0 is the null terminator
    while (guess[0] == 'h' || guess[0] == 't')
    {
        // build the flipcoin rpc
        strcpy(buff, "rpc=flipcoin;");
        strcat(buff, "guess=");
        strcat(buff, guess);
        strcat(buff, ";");

        // cout << "####inside bottom of Client.launchHeadsTails(), before sending. \nbuff is : " << buff << endl; 

        // sending game selection in buffer
        send(sockNum, buff , strlen(buff) , 0 );
        
        // clear the buffer before overwriting it with incoming buffer
        memset(buff, 0, 1024);
        // read message
        read(sockNum, buff, 1024);
        cout << "####PRINTING: inside bottom of Client.launchHeadsTails(), ~line 244, after reading. buffer received by Client: " << buff << endl;

        // TEMP CODE. TODO: parse this out.
        string coin = "HARDCODED_heads";

        cout << "The coin shows ___" << coin << "___. " << endl;
        cout << "Play again?  h)eads  t)ails.  Enter any other key to exit: ";
        cin >> guess;
    }

    cout << "That was extreme!!!" << endl;
    cout << "    // TODO: show game stats here: wins and rounds played" << endl;
    // TODO: show game stats here: wins and rounds played
}
