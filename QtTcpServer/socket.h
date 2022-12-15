#ifndef SOCKET_H
#define SOCKET_H

#include <QTimer>
#include <QObject>
#include <QDateTime>
#include <QTcpSocket>
#include <QDataStream>

#include "protocol.pb.h"

class Socket : public QObject {
    Q_OBJECT
public:
    explicit Socket(QObject * parent, qintptr descriptor);

public slots:
    void readyRead();

private slots:
    void slowResponse();

signals:
    void timeout(QTcpSocket *);
    void fastResponse(QTcpSocket * socket);

private:
    QTcpSocket * socket_;

    TestTask::Messages::WrapperMessage * msg;
    QTimer * timer;
};

#endif // SOCKET_H
