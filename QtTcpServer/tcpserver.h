#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>

#include "tcpsockethandler.h"
#include "../common/api.h"

class TcpServer : public QTcpServer {
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();
    bool listen(QHostAddress address, quint16 port);
    void close();

protected:
    void incomingConnection(qintptr descriptor) override;

private slots:
    void onSocketDisconnected(qintptr descriptor);

private:
    QMap<qintptr, TcpSocketHandler *> sockets;
};

#endif // TCPSERVER_H
