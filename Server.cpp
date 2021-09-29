
#include "Server.h"

Server::Server(int port)throw (const char*){
    this->Stop = 0;
    this->fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd<0)
        throw "socket failed";

    this->server.sin_family = AF_INET;
    this->server.sin_addr.s_addr = INADDR_ANY;
    this->server.sin_port = htons(port);

    if (bind(this->fd, (struct sockaddr *) &this->server, sizeof(this->server)) < 0) {
        throw "bind failure";
    }


    if(listen(fd, 5)<0)
        throw "listen failure";


}


void Server::start(ClientHandler& ch)throw(const char*){
        t = new thread([&ch, this]() {
            while (!this->Stop) {
                socklen_t cSize = sizeof(client);
                alarm(5);
                int clientFD = ::accept(this->fd, (struct sockaddr *) &client, &cSize);
                if (clientFD < 0) {
                    throw "accept failed";
                }
                ch.handle(clientFD);
                close(clientFD);
            }
            ::close(this->fd);
        });
}


void Server::stop(){
    this->Stop=1;
    t->join();
}

Server::~Server() {
}


//tom
