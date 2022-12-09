#include "socket.h"

Socket::Socket(QObject * parent, qintptr descriptor) : QObject{parent}, socket_(new QTcpSocket()), connected_time(QDateTime::currentDateTime().toTime_t()) {
    socket_->setSocketDescriptor(descriptor);
    connect(socket_, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket_, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket_, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(this, SIGNAL(processed(qintptr)), parent, SLOT(update(qintptr)));

    qDebug() << "client connected";
}

qint32 Socket::time() {
    qint32 delta = QDateTime::currentDateTime().toTime_t() - connected_time;
    return delta >= delay ? 0 : delay - delta;
}

void Socket::write(const char *msg) {
    socket_->write(msg);
}

void Socket::connected() {
    /*
        connected event
    */
}

void Socket::disconnected() {
    /*
        disconnected event
    */
    qDebug() << "client disconnected";
    socket_->deleteLater();
}

void Socket::readyRead() {
    QByteArray buffer = socket_->readAll();
    QString data(buffer);
    msg = new TestTask::Messages::WrapperMessage();
    msg->ParseFromString(data.toStdString());
    delay = msg->request_for_slow_response().time_in_seconds_to_sleep();
    emit processed(socket_->socketDescriptor());
}
