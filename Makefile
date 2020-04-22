CPPFLAGS = -std=c++11 -Wall -Werror -pedantic -ggdb

gcc server.cpp -o server.o
./server.o &

gcc client.cpp -o client.o
./client.o
