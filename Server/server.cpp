#include "server.h"

Server::Server(QObject *parent) : QTcpServer{parent} {
    connect(this, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void Server::start() {
    qDebug() << "Listening: " << this->listen(QHostAddress::Any, 8000);
}

void Server::newConnection() {
    qDebug() << "new connection";
    Socket * socket = new Socket(this->nextPendingConnection());
    socket->write("Hello");
}
