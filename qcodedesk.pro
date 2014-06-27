#-------------------------------------------------
#
# Project created by QtCreator 2014-06-26T09:52:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = qcodedesk
TEMPLATE = app

INCLUDEPATH += include

SOURCES += src/main.cpp \
    src/MainWindow.cpp \
    src/qcd/TrayIcon.cpp

HEADERS  += include/MainWindow.h \
    include/qcd/TrayIcon.h

FORMS    += \
    ui/mainwindow.ui

RESOURCES += \
    resources.qrc
