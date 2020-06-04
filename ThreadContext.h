#ifndef THREAD_CONTEXT_H
#define THREAD_CONTEXT_H

#include "ServerStats.h"

class ThreadContext {

private:
    int htWins;
    int htRounds;
    int winStreak;
    int lastGuess;
    
public:
    
    ThreadContext();

    ~ThreadContext();

    void incrementWins();
    void incrementRounds();
    void incrementWinStreak(ServerStats &);
    void resetWinStreak();
    void recordLastGuess(ServerStats &, int);

    int getWinStreak();
    int getWins();
    int getRounds();
    int getLastGuess();
};

#endif