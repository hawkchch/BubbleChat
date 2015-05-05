#-------------------------------------------------
#
# Project created by QtCreator 2015-05-05T16:06:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BubbleChat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chatmessageitem.cpp \
    chatmessagemodel.cpp \
    message.cpp \
    chatmessagedelegate.cpp

HEADERS  += mainwindow.h \
    chatmessageitem.h \
    chatmessagemodel.h \
    message.h \
    msgdef.h \
    chatmessagedelegate.h

FORMS    += mainwindow.ui

CONFIG   += c++11
