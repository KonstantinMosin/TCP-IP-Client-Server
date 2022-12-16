QT += core
QT += network
QT -= gui

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    ../common/func.cpp \
    ../common/message.pb.cc \
    tcpserver.cpp \
    tcpsockethandler.cpp

HEADERS += \
    ../common/func.h \
    ../common/message.pb.h \
    tcpserver.h \
    tcpsockethandler.h

LIBS += -L/usr/local/lib -lprotobuf
