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

int ThreadContext::getWins() { return htWins; }

int ThreadContext::getRounds() { return htRounds; }
