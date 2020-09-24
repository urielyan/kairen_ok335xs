#-------------------------------------------------
#
# Project created by QtCreator 2014-12-11T11:40:55
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = new
TEMPLATE = app

unix {
    INCLUDEPATH += ./common ./dataSave
}

include($$PWD/doubleElementMeachine/copy/new.pri)

SOURCES += main.cpp
