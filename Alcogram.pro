#-------------------------------------------------
#
# Project created by QtCreator 2017-11-04T14:09:38
#
#-------------------------------------------------

QT       += core gui
QT += xml

CONFIG += c++11

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
    circularProgressBar.cpp \
    configManager.cpp \
    page.cpp \
    stateMachine.cpp \
    splashScreen.cpp \
    splashScreenEternalSleep.cpp \
    splashScreenNoncriticalError.cpp \
    criticalErrorPage.cpp \
    noncriticalErrorPage.cpp \
    devices/posDeviceTest.cpp \
    devices/deviceManager.cpp

HEADERS  += mainWindow.h \
    startPageWidget.h \
    takePhotoPageWidget.h \
    page.h \
    payPageWidget.h \
    utils.h \
    alcoTestPageWidget.h \
    photoPrintPageWidget.h \
    circularProgressBar.h \
    configManager.h \
    stateMachine.h \
    splashScreen.h \
    splashScreenEternalSleep.h \
    splashScreenNoncriticalError.h \
    criticalErrorPage.h \
    noncriticalErrorPage.h \
    deviceInterfaces/cameraInterface.h \
    deviceInterfaces/posInterface.h \
    devices/posDeviceTest.h \
    devices/deviceManager.h \
    deviceInterfaces/baseDeviceInterface.h

FORMS    += mainWindow.ui \
    startPageWidget.ui \
    takePhotoPageWidget.ui \
    payPageWidget.ui \
    alcoTestPageWidget.ui \
    photoPrintPageWidget.ui \
    splashScreen.ui \
    splashScreenEternalSleep.ui \
    splashScreenNoncriticalError.ui \
    criticalErrorPage.ui \
    noncriticalErrorPage.ui

RESOURCES += \
    resources.qrc
