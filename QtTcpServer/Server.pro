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
    socket.cpp

HEADERS += \
    server.h \
    socket.h