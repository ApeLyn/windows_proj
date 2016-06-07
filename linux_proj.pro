#-------------------------------------------------
#
# Project created by QtCreator 2015-12-26T09:59:14
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport
QT       += webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = linux_proj
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    finddialog.cpp \
    findform.cpp \
    findreplacedialog.cpp \
    findreplaceform.cpp \
    highlighter.cpp \
    sundown/autolink.c \
    sundown/buffer.c \
    sundown/houdini_href_e.c \
    sundown/houdini_html_e.c \
    sundown/html.c \
    sundown/html_smartypants.c \
    sundown/markdown.c \
    sundown/stack.c \
    preferences.cpp

HEADERS  += mainwindow.h \
    findreplacedialog.h \
    findreplaceform.h \
    preferences.h \
    finddialog.h \
    findform.h \
    highlighter.h \
    preference.h \
    sundown/autolink.h \
    sundown/buffer.h \
    sundown/houdini.h \
    sundown/html.h \
    sundown/html_blocks.h \
    sundown/markdown.h \
    sundown/stack.h

FORMS    += mainwindow.ui \
    findreplacedialog.ui \
    findreplaceform.ui \
    preferences.ui

DISTFILES += \
    linux_proj.pro.user \
    .DS_Store \
    .gitignore \
    css/Clearness Dark.css \
    css/Clearness.css \
    css/GitHub.css \
    css/GitHub2.css

RESOURCES += \
    images.qrc
