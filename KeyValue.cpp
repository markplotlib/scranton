// TODO Figure out which of these is actually needed, copied from server 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h> 
#include <iostream>

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
		// assert(pch1);
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