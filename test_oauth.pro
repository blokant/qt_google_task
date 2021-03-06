#-------------------------------------------------
#
# Project created by QtCreator 2014-01-27T15:58:49
#
#-------------------------------------------------

QT       += core gui webkit network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_oauth
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    oauth2.cpp \
    logindialog.cpp \
    gtask.cpp \
    gtasklist.cpp \
    gtaskhelper.cpp

HEADERS  += dialog.h \
    oauth2.h \
    logindialog.h \
    gtask.h \
    gtasklist.h \
    gtaskhelper.h

FORMS    += dialog.ui
LIBS += -L/usr/local/lib -lqjson
