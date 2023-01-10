QT += core
QT += network
QT -= gui

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    ../common/cpp/delimitedmessagesstreamparser.cpp \
    ../common/cpp/api.cpp \
    ../common/cpp/message.pb.cc \
    tcpserver.cpp \
    tcpsockethandler.cpp

HEADERS += \
    ../common/cpp/delimitedmessagesstreamparser.h \
    ../common/cpp/api.h \
    ../common/cpp/message.pb.h \
    tcpserver.h \
    tcpsockethandler.h

LIBS += -L/usr/local/lib -lprotobuf
