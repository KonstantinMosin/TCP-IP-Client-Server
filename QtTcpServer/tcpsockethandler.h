#ifndef TCPSOCKETHANDLER_H
#define TCPSOCKETHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QDateTime>

#include "../common/delimitedmessagesstreamparser.h"
#include "../common/message.pb.h"
#include "../common/api.h"

class TcpSocketHandler : public QObject {
    Q_OBJECT
public:
    explicit TcpSocketHandler(QObject * parent = nullptr);
    ~TcpSocketHandler();
    bool setSocketDescriptor(qintptr descriptor);
    void setConnections(qint16 count);
    bool isOpen();
    void close();

signals:
    void socketDisconnected(qintptr descriptor);
    void fastResponse();
    void slowResponse();

private slots:
    void onReadyRead();
    void onFastResponse();
    void onSlowResponse();

private:
    typedef DelimitedMessagesStreamParser<WrapperMessage> Parser;
    Parser parser;
    std::vector<typename DelimitedMessagesStreamParser<WrapperMessage>::PointerToConstValue> messages;

    QTcpSocket * socket;
    QByteArray buffer;
    QTimer * timer;

    qint16 connections;
};

#endif // TCPSOCKETHANDLER_H
