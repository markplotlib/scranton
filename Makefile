#flag, can be either gcc or g++
CC=g++
CFLAGS= -std=c++11 -Wall -Werror -pedantic -ggdb

# default target for makefile
# simply type "make" in terminal
# and all will run
# syntax:
# [name]:[target]
# [tab][command]
all: server client

# type "make server" into terminal to only compile server
server: ServerGateway.cpp
	$(CC) $(CFLAGS) ServerGateway.cpp MainMenu.cpp ServerStats.cpp ServerStats.h StringParser.cpp ThreadContext.cpp ThreadContext.h StringParser.h KeyValue.cpp KeyValue.h HeadsTailsServer.cpp HeadsTailsServer.h GameClass2.cpp GameClass2.h ClientGame2.cpp -o serverGateway -lpthread
# same goes for "make client"
client: Client.cpp
	$(CC) $(CFLAGS) Client.cpp HeadsTailsClient.cpp -o client

# type "make clean" into the terminal here
# to only remove all .o file
# sometimes, compiler can't overwrite the old .o file
# so you have to delete them
# It's best practice to clear all .o files before any compile
clean:
	rm serverGateway client
