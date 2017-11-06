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
    takePhotoPageWidget.cpp \
    payPageWidget.cpp \
    utils.cpp \
    alcoTestPageWidget.cpp \
    photoPrintPageWidget.cpp \
    circularProgressBar.cpp

HEADERS  += mainWindow.h \
    startPageWidget.h \
    takePhotoPageWidget.h \
    page.h \
    payPageWidget.h \
    utils.h \
    alcoTestPageWidget.h \
    photoPrintPageWidget.h \
    circularProgressBar.h

FORMS    += mainWindow.ui \
    startPageWidget.ui \
    takePhotoPageWidget.ui \
    payPageWidget.ui \
    alcoTestPageWidget.ui \
    photoPrintPageWidget.ui

RESOURCES += \
    resources.qrc
