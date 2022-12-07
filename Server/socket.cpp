#include "socket.h"

Socket::Socket(QTcpSocket * socket) : socket_(socket) {

    connect(socket_, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket_, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket_, SIGNAL(readyRead()), this, SLOT(readyRead()));

    qDebug() << "client connected";
}

void Socket::write(const char *msg) {
    socket_->write(msg);
}

void Socket::connected() {
    qDebug() << "connected event";
}

void Socket::disconnected() {
    qDebug() << "disconnected event";
}

void Socket::readyRead() {
    qDebug() << socket_->readAll();
}
