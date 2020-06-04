#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <iostream>
#include "ThreadContext.h"

ThreadContext::ThreadContext()
{
    std::cout << "ThreadContext ctor" << std::endl;
    htWins = 0;
    htRounds = 0;
}

ThreadContext::~ThreadContext() { 
    std::cout << "ThreadContext dtor" << std::endl;
}

void ThreadContext::incrementWins() { htWins++; }

void ThreadContext::incrementRounds() { htRounds++; }

void ThreadContext::incrementWinStreak(ServerStats &sStats) {
    winStreak++; sStats.setHeadsTailsHS(winStreak);
}

void ThreadContext::resetWinStreak() { winStreak = 0; }

void ThreadContext::recordLastGuess(ServerStats &sStats, int lg) { 
    
    incrementRounds();
    lastGuess = lg;

    if (lg == 0) {
        resetWinStreak();
    }
    else 
    {
        incrementWins();
        incrementWinStreak(sStats);
        sStats.setHeadsTailsHS(winStreak);
    }

}


int ThreadContext::getWins() { return htWins; }

int ThreadContext::getRounds() { return htRounds; }

int ThreadContext::getWinStreak() { return winStreak; }

int ThreadContext::getLastGuess() { return lastGuess; }
