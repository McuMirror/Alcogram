#-------------------------------------------------
#
# Project created by QtCreator 2017-11-04T14:09:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Alcogram
TEMPLATE = app


SOURCES += main.cpp\
        mainWindow.cpp \
    startPageWidget.cpp \
    takePhotoPageWidget.cpp

HEADERS  += mainWindow.h \
    startPageWidget.h \
    takePhotoPageWidget.h \
    page.h

FORMS    += mainWindow.ui \
    startPageWidget.ui \
    takePhotoPageWidget.ui

RESOURCES += \
    resources.qrc
