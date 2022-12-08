#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QDateTime>
#include <QTcpSocket>
#include <QDataStream>

class Socket : public QObject {
    Q_OBJECT
public:
    explicit Socket(QObject * parent, qintptr descriptor);
    qint32 time();
    void write(const char * msg);

public slots:
    void connected();
    void disconnected();
    void readyRead();

signals:
    void processed(qintptr descriptor);

private:
    QTcpSocket * socket_;
    qint32 delay;
    qint32 connected_time;
};

#endif // SOCKET_H
