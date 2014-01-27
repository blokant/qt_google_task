#-------------------------------------------------
#
# Project created by QtCreator 2014-01-27T15:58:49
#
#-------------------------------------------------

QT       += core gui webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_oauth
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    oauth2.cpp \
    logindialog.cpp

HEADERS  += dialog.h \
    oauth2.h \
    logindialog.h

FORMS    += dialog.ui
