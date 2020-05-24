#ifndef STRING_PARSER_H
#define STRING_PARSER_H

#include "KeyValue.h"

class StringParser {

public:

    explicit StringParser(char *szUnformattedString);

    StringParser();

    ~StringParser();

    void newRPC(char *szUnformattedString);

    // The getNextKeyValue function
    // It will attempt to parse out part of the string all the way up to the ";",
    // it will then create a new KeyValue object  with that partial string
    // If it can;t it will return null;
    void getNextKeyValue(KeyValue & keyVal);

private:

    char rawString[32768]{};                         // The original string is copied here
    int m_currentPosition{};
    KeyValue *m_pKeyValue{};
    char *m_pch{};                                   // This is another copy of it
};

#endif