#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include "socket.h"

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void start();

private slots:
    void newConnection();
};

#endif // SERVER_H
