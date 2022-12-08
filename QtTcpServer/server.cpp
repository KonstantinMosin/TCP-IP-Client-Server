#include "server.h"

Server::Server(QObject *parent) : QTcpServer{parent}, timer(new QTimer(this)) {
    connect(timer, SIGNAL(timeout()), this, SLOT(idle()));
    timer->start();
}

void Server::start() {
    qDebug() << "Listening: " << this->listen(QHostAddress::Any, 8000);
}

void Server::incomingConnection(qintptr descriptor) {
    raw_sockets[descriptor] = new Socket(this, descriptor);
}

void Server::idle() {
    if (!avalaible_sockets.isEmpty() && !avalaible_sockets.first()->time()) {
        Socket * socket = avalaible_sockets.first();
        socket->write("Ok...");
        avalaible_sockets.erase(avalaible_sockets.begin());
    }
}

void Server::update(qintptr descriptor) {
    avalaible_sockets.insert(raw_sockets[descriptor]->time(), raw_sockets[descriptor]);
    qDebug() << avalaible_sockets.size();
}
