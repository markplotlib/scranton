
// key/value storage structure
class KeyValue {
    
public:
    KeyValue();

    void setKeyValue(char *pszBuff);

    char *getKey();

    char *getValue();

private:
    char m_szKey[128]{};
    char m_szValue[2048]{};
};

