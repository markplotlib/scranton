// TODO Figure out which of these is actually needed, copied from server 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include <iostream>
#include "KeyValue.cpp"

// This class will take a string that is passed to it in this format:
// input to constructor:
// <variable1>=<value1>;<variable2>=<value2>;
// You will then call the method  getNextKeyValue until getNextKeyValue returns NULL.
// getNextKeyValue will return a KeyValue object. Inside of that KeyValue object will contain the variable and the value
// You will then call getKey or getValue to get the contents of those fields.
// The example in main() will show how to call this function.
// By extracting the contents you then can determine the rpc you need to switch to, along with variables you will need
// You can also use this class in your client program, since you will need to determine the contents that you receive from server

class StringParser
{
private:

	char rawString[32768];                         // The original string is copied here
	int m_currentPosition;
	KeyValue *m_pKeyValue;
	char *m_pch;                                   // This is another copy of it

public:

	StringParser(char *szUnformattedString)
	{
		// assert(strlen(szUnformattedString));     // debugging code, makes sure that there's a string maybe?
		strcpy(rawString, szUnformattedString);     // copies the code, to prevent it from editing the original
		m_pch = rawString;                          // copies again, into a character array
	}

	StringParser()
	{}

	~StringParser()
	{
		if (m_pKeyValue)
			delete (m_pKeyValue);
	}

    void newRPC(char *szUnformattedString) {
        // assert(strlen(szUnformattedString));         // debugging code, makes sure that there's a string maybe?
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
		// assert(pch1 != NULL);                     // debugging: assert that pch1 doesn't point to null
		int subStringSize = (int)(pch1 - m_pch);  // creates a new string starting from pch1
		strncpy(szTemp, m_pch, subStringSize);    // copies the string from m_pch to szTemp, subStringSize characters
		szTemp[subStringSize] = 0;                // the final character is now 0; ???
		m_pch = pch1 + 1;
		if (m_pKeyValue)
			delete (m_pKeyValue);
		keyVal.setKeyValue(szTemp);               // sends szTemp to keyValue
	}

};