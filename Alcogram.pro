#-------------------------------------------------
#
# Project created by QtCreator 2017-11-04T14:09:38
#
#-------------------------------------------------

QT       += core gui
QT += xml
QT += sql

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = Alcogram
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

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
    devices/cameraDeviceTest.cpp \
    devices/cameraFrameGrabber.cpp \
    faceDetector.cpp \
    devices/alcoTesterDeviceTest.cpp \
    cameraImageHandler.cpp \
    devices/printerDeviceTest.cpp \
    logger.cpp \
    devices/machinery.cpp \
    deviceInterfaces/status.cpp \
    sessionData.cpp \
    devicesChecker.cpp

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
    deviceInterfaces/baseDeviceInterface.h \
    devices/cameraDeviceTest.h \
    devices/cameraFrameGrabber.h \
    interfaces/faceDetectionInterface.h \
    faceDetector.h \
    deviceInterfaces/alcotesterInterface.h \
    devices/alcotesterDeviceTest.h \
    cameraImageHandler.h \
    deviceInterfaces/printerinterface.h \
    devices/printerDeviceTest.h \
    logger.h \
    interfaces/mainWindowInterface.h \
    deviceInterfaces/status.h \
    devices/machinery.h \
    sessionData.h \
    devicesChecker.h

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

INCLUDEPATH += "C:/Program Files (x86)/dlib/include"
LIBS += -L"C:/Program Files (x86)/dlib/lib" -ldlib
