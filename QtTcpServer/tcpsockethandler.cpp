#include "tcpsockethandler.h"

TcpSocketHandler::TcpSocketHandler(QObject *parent) : QObject{parent} {
    timer = new QTimer();
    timer->setSingleShot(true);

    message = new WrapperMessage();
    size = 0;

    qDebug() << this << "created";
}

TcpSocketHandler::~TcpSocketHandler() {
    delete timer;
    delete message;

    qDebug() << this << "destroyed";
}

bool TcpSocketHandler::setSocketDescriptor(qintptr descriptor) {
    socket = new QTcpSocket();

    if (!socket->setSocketDescriptor(descriptor)) {
        qDebug() << "Socket underlying: " << descriptor << " descriptor already exist";
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

bool TcpSocketHandler::isOpen() {
    return socket->isOpen();
}

void TcpSocketHandler::close() {
    socket->close();
}

void TcpSocketHandler::onReadyRead() {
    buffer.append(socket->readAll());

    if (buffer.size() >= 4) {
        QSharedPointer<char> s(read(buffer, 4), [](char * s){ delete [] s; });
        size = convert_string_to_lu(s.data());
        buffer.remove(0, 4);
    }

    if (static_cast<quint32>(buffer.size()) >= size) {
        QSharedPointer<char> s(read(buffer, size), [](char * s){ delete [] s; });
        message = parse(s.data());
    }
    else {
        return;
    }

    if (message->has_request_for_fast_response()) {
        emit fastResponse();
    }
    else if (message->has_request_for_slow_response()) {
        timer->start(message->request_for_slow_response().time_in_seconds_to_sleep() * 1000);
    }
    else {
        qDebug() << "Unknown message";
        socket->write("Unknown message");
    }
}

void TcpSocketHandler::onSlowResponse() {
    socket->write(slow_response(connections).data());
}

void TcpSocketHandler::onFastResponse() {
    socket->write(fast_response().data());
}
