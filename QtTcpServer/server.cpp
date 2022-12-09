#include "server.h"

Server::Server(QObject *parent, QString path) : QTcpServer{parent}, port(8000) {
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QString line = file.readAll();
        port = line.toUInt();
        file.close();
    }
}

void Server::start() {
    qDebug() << "Listening: " << this->listen(QHostAddress::Any, port) << " at port " << port;
}

void Server::incomingConnection(qintptr descriptor) {
    sockets[descriptor] = new Socket(this, descriptor);
}

void Server::fastResponse(QTcpSocket* socket) {
    socket->write("BOBUS");
}

void Server::slowResponse(QTcpSocket* socket) {
    socket->write("ABOBA");
}
