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
server: server.cpp
	$(CC) $(CFLAGS) server.cpp -o server
# same goes for "make client"
client: client.cpp
	$(CC) $(CFLAGS) client.cpp -o client

# type "make clean" into the terminal here
# to only remove all .o file
# sometimes, compiler can't overwrite the old .o file
# so you have to delete them
# It's best practice to clear all .o files before any compile
clean:
	rm server client