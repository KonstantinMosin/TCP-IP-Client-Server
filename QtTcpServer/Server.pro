QT += core
QT += network
QT -= gui

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    ../common/delimitedmessagesstreamparser.cpp \
    ../common/api.cpp \
    ../common/message.pb.cc \
    tcpserver.cpp \
    tcpsockethandler.cpp

HEADERS += \
    ../common/delimitedmessagesstreamparser.h \
    ../common/api.h \
    ../common/message.pb.h \
    tcpserver.h \
    tcpsockethandler.h

LIBS += -L/usr/local/lib -lprotobuf
