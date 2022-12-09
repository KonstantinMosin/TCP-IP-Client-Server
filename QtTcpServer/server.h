#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
//#include <QTextStream>
#include <QDateTime>
#include <QMultiMap>
#include <QObject>
#include <QTimer>
#include <QFile>
#include <QMap>

#include "socket.h"

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr, QString path = "");
    void start();

protected:
    void incomingConnection(qintptr descriptor) Q_DECL_OVERRIDE;

private slots:
    void fastResponse(QTcpSocket* socket);
    void slowResponse(QTcpSocket* socket);

private:
    QMap<qintptr, Socket *> sockets;
    qint32 port;
};

#endif // SERVER_H
