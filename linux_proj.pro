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
    findreplaceform.cpp \
    preferences.cpp

HEADERS  += mainwindow.h \
    finddialog.h \
    findform.h \
    findreplacedialog.h \
    findreplaceform.h \
    preferences.h

FORMS    += mainwindow.ui \
    findreplacedialog.ui \
    findreplaceform.ui \
    preferences.ui

DISTFILES += \
    linux_proj.pro.user

<<<<<<< HEAD
RESOURCES += \
    images.qrc
=======
RESOURCES +=
>>>>>>> 49541b065bca3ca4b8fc0a3c87bd3474d44b777d
