#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDateTime>
#include <QMultiMap>
#include <QObject>
#include <QTimer>
#include <QMap>

#include "socket.h"

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void start();

protected:
    void incomingConnection(qintptr descriptor) Q_DECL_OVERRIDE;

protected slots:
    void update(qintptr descriptor);

private slots:
    void idle();

private:
    QMap<qintptr, Socket *> raw_sockets;
    QMultiMap<qint32, Socket *> avalaible_sockets;
    QTimer * timer;
};

#endif // SERVER_H
