/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <iostream>
#include <pthread.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "commands.h"
#include "CLI.h"
#include <signal.h>
#include <err.h>
#include <mutex>
#include <atomic>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        SocketIO s(clientID);
        CLI cli(&s);
        cli.start();
    }
};



// implement on Server.cpp
class Server {
	thread* t;

	// you may add data members
    int fd;
    volatile int Stop=0;
	sockaddr_in server;
	sockaddr_in client;

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
