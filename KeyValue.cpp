#include "KeyValue.h"
#include <cstring>

KeyValue::KeyValue() = default;

void KeyValue::setKeyValue(char *pszBuff) {

    char *pch1;

    // find out where the "=" sign is, and take everything to the left of the equal for the key
    // go one beyond the = sign, and take everything else
    pch1 = strchr(pszBuff, '=');
    // assert(pch1);
    int keyLen = (int) (pch1 - pszBuff);
    strncpy(m_szKey, pszBuff, keyLen);
    m_szKey[keyLen] = 0;
    strcpy(m_szValue, pszBuff + keyLen + 1);

}

char *KeyValue::getKey() {

    return m_szKey;

}

char *KeyValue::getValue() {

    return m_szValue;

}