#ifndef TCPSOCKETHANDLER_H
#define TCPSOCKETHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QDateTime>

#include "../common/message.pb.h"
#include "../common/func.h"

class TcpSocketHandler : public QObject {
    Q_OBJECT
public:
    explicit TcpSocketHandler(QObject * parent = nullptr);
    ~TcpSocketHandler();
    bool setSocketDescriptor(qintptr descriptor);
    void setConnections(qint16 count);

signals:
    void socketDisconnected(qintptr descriptor);
    void fastResponse();
    void slowResponse();

private slots:
    void onReadyRead();
    void onFastResponse();
    void onSlowResponse();

private:
    QTcpSocket * socket;
    QByteArray buffer;
    QTimer * timer;

    WrapperMessage * message;
    quint32 size;

    qint16 connections;
};

#endif // TCPSOCKETHANDLER_H
