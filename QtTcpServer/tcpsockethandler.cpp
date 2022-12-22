#include "tcpsockethandler.h"

TcpSocketHandler::TcpSocketHandler(QObject *parent) : QObject{parent} {
    buffer = new QByteArray();
    timer = new QTimer();
    timer->setSingleShot(true);

    message = new WrapperMessage;
    size = 0;

    qDebug() << this << "created";
}

TcpSocketHandler::~TcpSocketHandler() {
    delete buffer;
    delete timer;

    qDebug() << this << "destroyed";
}

bool TcpSocketHandler::setSocketDescriptor(qintptr descriptor) {
    socket = new QTcpSocket();

    if (!socket->setSocketDescriptor(descriptor)) {
        //handle warning
        qDebug() << "socket underlying";
        socket->deleteLater();
        return false;
    }

    connect(socket, &QTcpSocket::readyRead, this, &TcpSocketHandler::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, [this, descriptor](){ emit socketDisconnected(descriptor); });

    connect(this, &TcpSocketHandler::fastResponse, this, &TcpSocketHandler::onFastResponse);
    connect(this, &TcpSocketHandler::slowResponse, this, &TcpSocketHandler::onSlowResponse);
    connect(timer, &QTimer::timeout, this, [this](){ emit slowResponse(); });

    return true;
}

void TcpSocketHandler::setConnections(qint16 count) {
    connections = count;
}

void TcpSocketHandler::onReadyRead() {
    buffer->append(socket->readAll());

    if (!size && buffer->size() >= 4) {
        size = buffer->left(4).toInt();
        buffer->remove(0, 4);
    }

    if (size <= 0) {
        qDebug() << "Wrong message size";
        //handle error
        return;
    }

    if (buffer->size() < size) {
        qDebug() << "Message not ready to read";
        return;
    }

    QString data = QString(buffer->left(size));
    message->ParseFromString(data.toStdString());

    if (message->has_request_for_fast_response()) {
        emit fastResponse();
    }
    else if (message->has_request_for_slow_response()) {
        timer->start(message->request_for_slow_response().time_in_seconds_to_sleep() * 1000);
    }
    else {
        qDebug() << "Unknown message";
        //handle error
    }
}

void TcpSocketHandler::onSlowResponse() {
    socket->write(slow_response(connections).data());
}

void TcpSocketHandler::onFastResponse() {
    socket->write(fast_response().data());
}
