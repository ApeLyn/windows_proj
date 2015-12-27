#-------------------------------------------------
#
# Project created by QtCreator 2015-12-26T09:59:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = linux_proj
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    finddialog.cpp \
    findform.cpp \
    findreplacedialog.cpp \
    findreplaceform.cpp

HEADERS  += mainwindow.h \
    finddialog.h \
    findform.h \
    findreplacedialog.h \
    findreplaceform.h

FORMS    += mainwindow.ui \
    findreplacedialog.ui \
    findreplaceform.ui

DISTFILES += \
    linux_proj.pro.user
