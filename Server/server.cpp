#include "server.h"

Server::Server(QObject *parent) : QObject{parent} {
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    qDebug() << "Listening:" << server->listen(QHostAddress::Any, 8000);
}

void Server::newConnection() {
    QTcpSocket * socket = server->nextPendingConnection();

    socket->write("Greetings from server");
    socket->flush();

    socket->waitForBytesWritten(3000);
    socket->close();
}
