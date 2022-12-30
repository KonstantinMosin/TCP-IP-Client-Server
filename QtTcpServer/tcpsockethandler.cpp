#include "tcpsockethandler.h"

TcpSocketHandler::TcpSocketHandler(QObject *parent) : QObject{parent} {
    timer = new QTimer();
    timer->setSingleShot(true);

    qDebug() << this << "created";
}

TcpSocketHandler::~TcpSocketHandler() {
    delete timer;

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
    printf("reading\n");
    for (const char byte : socket->readAll()) {
        const std::list<typename DelimitedMessagesStreamParser<WrapperMessage>::PointerToConstValue> & parsedMessages = parser.parse(std::string(1, byte));
        for (const DelimitedMessagesStreamParser<WrapperMessage>::PointerToConstValue & message : parsedMessages) {
            messages.push_back(message);
        }
    }

    for (const auto & message : messages) {
        if (message->has_request_for_fast_response()) {
            printf("emit fast response\n");
            emit fastResponse();
        }
        else if (message->has_request_for_slow_response()) {
            printf("emit slow response\n");
            timer->start(message->request_for_slow_response().time_in_seconds_to_sleep() * 1000);
        }
        else {
            socket->write("unknown message");
        }
    }
    printf("stop reading\n");
}

void TcpSocketHandler::onSlowResponse() {
    printf("send slow response\n");
    socket->write(slow_response(connections).data());
}

void TcpSocketHandler::onFastResponse() {
    printf("send fast response\n");
    socket->write(fast_response().data());
}
