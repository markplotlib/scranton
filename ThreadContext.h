#ifndef THREAD_CONTEXT_H
#define THREAD_CONTEXT_H

class ThreadContext {

private:
    int htWins;
    int htRounds;

public:
    
    ThreadContext();

    ~ThreadContext();

    void incrementWins();
    void incrementRounds();

    int getWins();
    int getRounds();

};

#endif