QT += core
QT += network
QT -= gui

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    server.cpp \
    socket.cpp \
    protocol.pb.cc

HEADERS += \
    server.h \
    socket.h \
    protocol.pb.h

LIBS += -L/usr/local/lib -lprotobuf
