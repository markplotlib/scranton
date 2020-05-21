// Client side C/C++ program to demonstrate Socket programming 
// ports: Peter 12117, Jara 12115, Mark 12104
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <iostream>
#define PORT 8080
using namespace std;

// get usrer choice for menu switch
int getUserChoice() {
    int userChoice; 
    cin >> userChoice; 
    return userChoice; 
}

// display menu options to the user 
void displayUserMenu() {
    cout << "\nMenu Options\n";
    cout << "\nSelect from the following options\n";
    cout << "1 - Disconnect\n";
    cout << "2 - Game Menu\n";
    // TODO: add game menu/ more options 
    cout << "User Selection: ";
}

// add the user arguments to the buffer 
void login(char* buffer, char* username, char* password){
    strcpy(buffer, "rpc=connect;");  // take note: this is strcpy, not strcat! (easy to overlook)
    strcat(buffer, "user=");
    strcat(buffer, username);
    strcat(buffer, ";password=");
    strcat(buffer, password);
    strcat(buffer, ";");
    puts(buffer);  // another way to print to screen
}

// get credentials from the user 
void getUserCredentials(char* username, char* password){
    cout << "\nPlease enter username ('mike'): ";
    cin >> username;
    cout << "Please enter password ('123'): ";
    cin >> password;
    cout << "\nYou've typed " << username << " and " << password << endl;
}


int main(int argc, char** argv) { 
    // to store user choice 
    int choice = 0; 
    int sock = 0;
    struct sockaddr_in serv_addr; 
    char buffer[1024] = {0}; 
    char argv1[40];
    char argv2[40]; 
    //char argv1, argv2;
    // char[40] argv2;

    getUserCredentials(argv1, argv2);
/*
    cout << "\nWelcome to the Dunder Mifflin client portal.\n" << endl;
    cout << "\nPlease enter username ('mike'): ";
    cin >> argv1;
    //argv1 = "mike";
    cout << "Please enter password ('123'): ";
    cin >> argv2;
    //argv2 = "123";
    cout << "\nYou've typed " << argv1 << " and " << argv2 << endl;

    // add the user arguments to the buffer
    strcpy(buffer, "rpc=connect;");  // take note: this is strcpy, not strcat! (easy to overlook)
    strcat(buffer, "user=");
    strcat(buffer, argv1);
    //strcat(buffer, "mike");
    strcat(buffer, ";password=");
    strcat(buffer, argv2);
    //strcat(buffer, "123");
    strcat(buffer, ";");
    puts(buffer);  // another way to print to screen
*/

    // user login 
    login(buffer, argv1, argv2); 

    //TODO: Do we remove sleep completely?
    //srand (time(NULL));    
    const int SLEEP_TIME = 2;  //rand() % 10 + 1;  // Sleep for 1 to 10 seconds

/*
    int sock = 0;
    struct sockaddr_in serv_addr; 
    char buffer[1024] = {0}; 

    strcpy(buffer, "rpc=connect;");  // take note: this is strcpy, not strcat! (easy to overlook)
    strcat(buffer, "user=");
    // strcat(buffer, argv1);
    strcat(buffer, "mike");
    strcat(buffer, ";password=");
    // strcat(buffer, argv2);
    strcat(buffer, "123");
    strcat(buffer, ";");
    puts(buffer);  // another way to print to screen
*/
    const char *DISCONNECT_RPC = "disconnected"; // thisfix: DISCONNECT_RPC = "disconnect". NOTE: the discrepancy/ambiguity: "disconnect" is the rpc command. "disconnected" is displayed to user.
    // char DISCONNECT_MSG[1024] = {0}; 
    
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

    // TODO: please fix! should not always print!
    // int statusCode = 0; // 0 = no error
    // if (statusCode == 0)
    // 	printf("\nWelcome %s", argv[1]);

    // read incoming message
    memset(buffer, 0, sizeof(buffer));
    read(sock, buffer, 1024); // remember: read returns an int, corresponding the number of characters entered

        //cout << DISCONNECT_RPC << endl;
        //cout << buffer; 
    // cout << "debug statement: show buffer" << buffer << endl;
    if (strcmp(buffer, DISCONNECT_RPC) == 0) { // thisfix: if (strcmp(rpcKey, "rpc") == 0 && strcmp(rpcValue, "disconnect") == 0) {}
         // server is in listening loop
        //cout << DISCONNECT_RPC << endl;
        //cout << buffer; 
        cout << "\nWrong credentials, disconnecting\n"; 
        // TODO: Remove sleep time if necessary
        cout << "Auto-disconnect in " << SLEEP_TIME << " seconds";
        cout.flush();
        sleep(SLEEP_TIME);
        cout << endl;


        return 0;
    } else {
        /*
        cout << "============" << endl;
        cout << "buffer:" <<  buffer << endl;
        cout << "============" << endl;
        cout << "DISCONNECT_RPC: " << DISCONNECT_RPC << endl;
        */
    }

    
// Client start menu loop 
do { 
    // clear screen goes here
    displayUserMenu(); 
    choice = getUserChoice(); 
    switch(choice) {
            case 1: 
                cout << "\nDisconnecting from the Server\n"; 
                // Send choice to server to disconnect
                send(sock , DISCONNECT_RPC , strlen(DISCONNECT_RPC) , 0 );
                printf("Disconnect message sent\n");    
                read(sock, buffer, 1024);
                break; 
            case 2: 
                cout << "\nOpening Game Menu\n"; 
                // TODO: Make game menu, open here 
                break;
            default:
                break; 
    }
} while(choice != 1);

//TODO: Remove sleep timer?
// sleep timer 
    cout << "Auto-disconnect in " << SLEEP_TIME << " seconds";
    cout.flush();
    sleep(SLEEP_TIME);
    cout << endl;

// Disconnects 

    // server is in listening loop
/*
    send(sock , DISCONNECT_RPC , strlen(DISCONNECT_RPC) , 0 );
    printf("Disconnect message sent\n");    

    read(sock, buffer, 1024);
*/
    return 0; 
} 

// thisfix:
void logout(char* buffer) {
    strcpy(buffer, "rpc=disconnect;");  // take note: this is strcpy, not strcat! (easy to overlook)
    puts(buffer);  // another way to print to screen
}
