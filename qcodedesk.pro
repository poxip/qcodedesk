#-------------------------------------------------
#
# Project created by QtCreator 2014-06-26T09:52:10
#
#-------------------------------------------------

QT       += core network xml gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 no_keywords

TARGET = qcodedesk
TEMPLATE = app

INCLUDEPATH += include

SOURCES += src/main.cpp \
    src/MainWindow.cpp \
    src/qcd/TrayIcon.cpp \
    src/News.cpp \
    src/Topic.cpp

HEADERS  += include/MainWindow.h \
    include/qcd/TrayIcon.h \
    include/News.h \
    include/Topic.h \
    include/config.h

FORMS    += \
    ui/mainwindow.ui

RESOURCES += \
    resources.qrc

unix:!macx {
    CONFIG += link_pkgconfig
    PKGCONFIG += libnotify
}
