#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class Server : public QObject {
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

signals:

private slots:
    void newConnection();

private:
    QTcpServer * server;
};

#endif // SERVER_H
