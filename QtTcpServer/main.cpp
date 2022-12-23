#include <QCoreApplication>
#include <QFile>

#include "tcpserver.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    TcpServer server;

    QHostAddress address = QHostAddress("127.0.0.1");
    quint16 port = 8000;

    QFile file("../port.txt");
    if (file.open(QIODevice::ReadOnly)) {
        QString line = file.readAll();
        port = line.toUInt();
        file.close();
    }
    else {
        qDebug() << "File would not be open";
    }

    if (!server.listen(address, port)) {
        qDebug() << "Server would not be listen";
    }

    return a.exec();
}
