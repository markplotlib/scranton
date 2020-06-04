CC=g++
CFLAGS= -std=c++11 -Wall -Werror -pedantic -ggdb

all: server client

server: ServerGateway.cpp
	$(CC) $(CFLAGS) ServerGateway.cpp MainMenu.cpp ServerStats.cpp ServerStats.h StringParser.cpp ThreadContext.cpp ThreadContext.h StringParser.h KeyValue.cpp KeyValue.h HeadsTailsServer.cpp HeadsTailsServer.h GameClass2.cpp GameClass2.h ClientGame2.cpp -o serverGateway -lpthread
# type "make client" to only compile client files below
client: Client.cpp
	$(CC) $(CFLAGS) Client.cpp HeadsTailsClient.cpp -o client

# type "make clean" to only remove binary executable files (best practice before compiling)
clean:
	rm serverGateway client
