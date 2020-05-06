// Server side C/C++ program to demonstrate Socket programming 
// ports: Peter 12117, Jara 12115, Mark 12104
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <iostream>
// next line from MM
#include "assert.h"
#define PORT 12104
using namespace std;

// key/value storage structure
class KeyValue
{
private:
	char m_szKey[128];
	char m_szValue[2048];
public:
	KeyValue() {};
	void setKeyValue(char *pszBuff) {	
		char *pch1;

		// find out where the "=" sign is, and take everything to the left of the equal for the key
		// go one beyond the = sign, and take everything else
		pch1 = strchr(pszBuff, '=');
		assert(pch1);
		int keyLen = (int)(pch1 - pszBuff);
		strncpy(m_szKey, pszBuff, keyLen);
		m_szKey[keyLen] = 0;
		strcpy(m_szValue, pszBuff + keyLen + 1);
	}

	char *getKey() {
		return m_szKey;
	}

	char *getValue() {
		return m_szValue;
	}

};


// This class will take a string that is passed to it in this format:

// input to constructor:
// <variable1>=<value1>;<variable2>=<value2>;
//You will then call the method  getNextKeyValue until getNextKeyValue returns NULL.
// getNextKeyValue will return a KeyValue object. Inside of that KeyValue object will contain the variable and the value
// You will then call getKey or getValue to get the contents of those fields.
// The example in main() will show how to call this function.
// By extracting the contents you then can determine the rpc you need to switch to, along with variables you will need
// You can also use this class in your client program, since you will need to determine the contents that you receive from server

class Interpreter
{
private:

	char rawString[32768];                         // The original string is copied here
	int m_currentPosition;
	KeyValue *m_pKeyValue;
	char *m_pch;                                   // This is another copy of it
public:

	Interpreter(char *szUnformattedString)
	{
		assert(strlen(szUnformattedString));            // debugging code, makes sure that there's a string maybe?
		strcpy(rawString, szUnformattedString);     // copies the code, to prevent it from editing the original
		m_pch = rawString;                          // copies again, into a character array
	}

	Interpreter()
	{}

	~Interpreter()
	{
		if (m_pKeyValue)
			delete (m_pKeyValue);
	}

    void newRPC(char *szUnformattedString) {
        assert(strlen(szUnformattedString));         // debugging code, makes sure that there's a string maybe?
		strcpy(rawString, szUnformattedString);     // copies the code, to prevent it from editing the original
		m_pch = rawString;
    }

	void getNextKeyValue(KeyValue & keyVal)
	{
		// It will attempt to parse out part of the string all the way up to the ";", it will then create a new KeyValue object  with that partial string
		// If it can;t it will return null;
		char *pch1;                               // pointer character 1
		char szTemp[32768];                       

		pch1 = strchr(m_pch, ';');                // returns a pointer to the first instance of ;
		assert(pch1 != NULL);                     // debugging: assert that pch1 doesn't point to null
		int subStringSize = (int)(pch1 - m_pch);  // creates a new string starting from pch1
		strncpy(szTemp, m_pch, subStringSize);    // copies the string from m_pch to szTemp, subStringSize characters
		szTemp[subStringSize] = 0;                // the final character is now 0; ???
		m_pch = pch1 + 1;
		if (m_pKeyValue)
			delete (m_pKeyValue);
		keyVal.setKeyValue(szTemp);               // sends szTemp to keyValue

	}

};


int main(int argc, char const *argv[]) 
{ 

    int server_fd, new_socket;
    // int valread;
    struct sockaddr_in address;
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};
    // const char *HELLO = "Hello from server";
    // char HELLO[1024] = "Hello from server"; 
    char DISCONNECT_RPC[1024] = "disconnect"; 
    char DISCONNECT_MSG[1024] = {0}; 
    
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                             &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );       // htons = host to network short. Accounts for computer-side short storage peculiarities.
    
    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, 
                                sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    // this loop is the server remaining active 
    bool disconnectFlag = false;
    while (true) {

        // Establish a new connection.
        if (listen(server_fd, 3) < 0)
        { 
            cout << endl << "Listen error" << endl;
            perror("listen");
            exit(EXIT_FAILURE); 
        } 
        // new_socket is where we'll be communicating to the client
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                            (socklen_t*)&addrlen))<0) 
        { 
            cout << endl << "accept error" << endl;
            perror("accept");
            exit(EXIT_FAILURE); 
        }


        // TODO: put initial login RPC logic here
        /*
        takes the incoming login information.
        Passes it to the interpreter
        Interpreter ....
        */

        // setup
        Interpreter *interpreter = new Interpreter();   // previous: (char *)testMSG
        KeyValue rpcKeyValue;
        
        char *rpcKey;
	    char *rpcValue;

        read(new_socket, buffer, 1024);

        interpreter->newRPC(buffer);                    // give the interpreter the message  
        interpreter->getNextKeyValue(rpcKeyValue);      // assign Key/Value data structure the first pair

        rpcKey = rpcKeyValue.getKey();
        rpcValue = rpcKeyValue.getValue();

        // revisit this for refactoring
        // rpcKey = "rpc";
        // rpcValue = "connect";

        // TODO: ensure that statusCode is returned (int), on client side, -1
        const char *CORRECT_UN = "mike";
        const char *CORRECT_PW = "123";

        if (strcmp(rpcKey, "rpc") == 0) {
            if (strcmp(rpcValue, "connect") == 0) {

                // Get the next two arguments (user and password);
                KeyValue userKeyValue;
                KeyValue passKeyValue;

                //char *pszUserKey;       // should match "user"
                char *pszUserValue;     // should match *CORRECT_UN
                //char *pszPassKey;       // should match "password"
                char *pszPassValue;     // should match *CORRECT_PW
                // int status;

                interpreter->getNextKeyValue(userKeyValue);
                // pszUserKey = userKeyValue.getKey();
                pszUserValue = userKeyValue.getValue();

                interpreter->getNextKeyValue(passKeyValue);
                // pszPassKey = passKeyValue.getKey();
                pszPassValue = passKeyValue.getValue();

                if (strcmp(pszUserValue, CORRECT_UN) == 0) {
                    cout << "Validated: username=" << pszUserValue << ". ";
                    if (strcmp(pszPassValue, CORRECT_PW) == 0)
                        cout << "Successful login." << endl;
                    else
                        cout << "Unsuccessful login: bad password entered." << endl;
                } else
                    cout << "Unsuccessful login: user does not exist." << endl;

                cout << "\n\n\n";
// step 0: (now) code that works
// step 1: (milestone 1) connect is a function
// step 2: (milestone 2) func in a class

                // step 1: 
                // status = Connect(pszUserValue, pszPassValue);
            }
        } else {
            // send disconnect
        }

        // if login credentials match, send either connect conf, or disconnect.
// TODO: SEND A BINARY VALUE
        send(new_socket , buffer , strlen(buffer) , 0 );

        while (!disconnectFlag) {

                // valread = (would equal size of message)
                // take in an RPC
                read(new_socket, buffer, 1024);



                if (*buffer == *DISCONNECT_RPC) {
                    send(new_socket , DISCONNECT_MSG , strlen(buffer) , 0 ); 
                    disconnectFlag = true;
                    // close active socket
                    close(new_socket);
                } else {
                    // message sent back to client.
                    send(new_socket , buffer , strlen(buffer) , 0 ); 
                }
                // TODO might not be necessary
                memset(buffer, 0, sizeof(buffer));
            }


            disconnectFlag = false;
        }   // end of always on loop



    return 0; 
} 
