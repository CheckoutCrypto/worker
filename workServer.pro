#-------------------------------------------------
#
# Project created by QtCreator 2014-12-10T20:35:38
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = workServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += src/main.cpp \
    src/server/server.cpp \
    src/server/qmyserver.cpp \
    src/threads/serverthread.cpp \
    src/threads/mainthread.cpp \
    src/threads/control.cpp \
    src/db/cache.cpp \
    src/lib/menu.cpp \
    src/objects/daeobj.cpp \
    src/objects/dbobj.cpp \
    src/objects/grpobj.cpp \
    src/objects/wallet.cpp \
    src/lib/simplecrypt.cpp \
    src/objects/wrkobj.cpp \
    src/threads/controlthread.cpp \
    src/db/dataconnect.cpp \
    src/lib/jsoncalls.cpp \
    src/lib/rpccall.cpp \
    src/threads/parsewrite.cpp \
    src/lib/rpccntrl.cpp \
    src/threads/orderthread.cpp

HEADERS += \
    src/server/server.h \
    src/server/qmyserver.h \
    src/threads/serverthread.h \
    src/threads/mainthread.h \
    src/threads/control.h \
    src/db/cache.h \
    src/lib/menu.h \
    src/objects/daeobj.h \
    src/objects/dbobj.h \
    src/objects/grpobj.h \
    src/objects/wallet.h \
    src/lib/simplecrypt.h \
    src/objects/wrkobj.h \
    src/threads/controlthread.h \
    src/db/dataconnect.h \
    src/lib/jsoncalls.h \
    src/lib/rpccall.h \
    src/threads/parsewrite.h \
    src/lib/rpccntrl.h \
    src/threads/orderthread.h
