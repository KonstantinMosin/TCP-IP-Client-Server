#include "tcpserver.h"
#include <QThread>

TcpServer::TcpServer(QObject *parent) : QTcpServer{parent} {
    qDebug() << this << "created";
}

TcpServer::~TcpServer() {
    qDebug() << this << "destroed";
}

bool TcpServer::listen(QHostAddress address, quint16 port) {
    if (!QTcpServer::listen(address, port)) return false;
    qDebug() << "Server listen " << address << " on " << port << " port";
    return true;
}

void TcpServer::close() {
    QTcpServer::close();
}

void TcpServer::incomingConnection(qintptr descriptor) {
    TcpSocketHandler * handler = new TcpSocketHandler();

    if (!handler->setSocketDescriptor(descriptor)) {
        //handle error
        handler->deleteLater();
        return;
    }

    sockets[descriptor] = handler;
    handler->setConnections(sockets.size());
    connect(handler, SIGNAL(socketDisconnected(qintptr)), this, SLOT(onSocketDisconnected(qintptr)));
}

void TcpServer::onSocketDisconnected(qintptr descriptor) {
    delete sockets[descriptor];
    sockets.remove(descriptor);
}
