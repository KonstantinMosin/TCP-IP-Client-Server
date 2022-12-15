QT += core
QT += network
QT -= gui

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    message.pb.cc \
    tcpserver.cpp \
    tcpsockethandler.cpp

HEADERS += \
    message.pb.h \
    tcpserver.h \
    tcpsockethandler.h

LIBS += -L/usr/local/lib -lprotobuf
