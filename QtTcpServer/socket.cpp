#include "socket.h"

Socket::Socket(QObject * parent, qintptr descriptor) : QObject{parent}, socket_(new QTcpSocket()), timer(new QTimer()) {
    socket_->setSocketDescriptor(descriptor);
    connect(socket_, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(this, SIGNAL(fastResponse(QTcpSocket *)), parent, SLOT(fastResponse(QTcpSocket *)));
    connect(timer, SIGNAL(timeout()), this, SLOT(slowResponse()));
    connect(this, SIGNAL(timeout(QTcpSocket *)), parent, SLOT(slowResponse(QTcpSocket *)));

    qDebug() << "client connected";
}

void Socket::readyRead() {
    QByteArray buffer = socket_->readAll();
    QString data(buffer);
    msg = new TestTask::Messages::WrapperMessage();
    msg->ParseFromString(data.toStdString());
    if (msg->has_request_for_fast_response()) {
        emit fastResponse(socket_);
    }
    else {
        quint32 delay = msg->request_for_slow_response().time_in_seconds_to_sleep();
        timer->setSingleShot(true);
        timer->start(delay * 1000);
    }
}

void Socket::slowResponse() {
    emit timeout(socket_);
}
