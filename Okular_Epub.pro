#-------------------------------------------------
#
# Project created by QtCreator 2013-05-04T02:24:09
#
#-------------------------------------------------

QT       += core gui webkit network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Okular_Epub
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    epubdocument.cpp \
    epubnetworkmanager.cpp

HEADERS  += mainwindow.h \
    epubdocument.h \
    epubnetworkmanager.h

INCLUDEPATH += /usr/include

LIBS += -lepub

FORMS    += mainwindow.ui
