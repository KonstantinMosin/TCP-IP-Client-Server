#ifndef TCPSOCKETHANDLER_H
#define TCPSOCKETHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

#include "message.pb.h"

class TcpSocketHandler : public QObject {
    Q_OBJECT
    typedef TestTask::Messages::WrapperMessage Message;
public:
    explicit TcpSocketHandler(QObject * parent = nullptr);
    ~TcpSocketHandler();
    bool setSocketDescriptor(qintptr descriptor);

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
    QByteArray * buffer;
    QTimer * timer;

    Message * message;
    qint32 size;
};

#endif // TCPSOCKETHANDLER_H
