#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QTcpSocket>

class Socket : public QObject {
    Q_OBJECT
public:
    explicit Socket(QTcpSocket * socket = nullptr);
    void write(const char * msg);

public slots:
    void connected();
    void disconnected();
    void readyRead();

private:
    QTcpSocket * socket_;
};

#endif // SOCKET_H
