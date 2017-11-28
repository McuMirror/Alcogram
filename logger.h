#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QStringList>

#include "interfaces/mainwindowinterface.h"

class Logger : public QObject
{
    Q_OBJECT
public:
    enum Action {
        BUTTON_RELEASE
    };

    enum Device {
        NONE
        , BUTTON
        , ALCOTESTER
        , CAMERA
        , POS
        , PRINTER
    };

    enum Event {
        SYSTEM_CHECK
        , USER_TIMER_START
        , USER_TIMER_STOP
        , SYSTEM_TIMER_START
        , SYSTEM_TIMER_STOP
        , ERROR
    };


    ~Logger();

    static Logger* instance();
    static void resetInstance();

    void setMainWindow(MainWindowInterface* mainWindow);
    void log(const QString& query);

    QString buildTapLog(int x, int y) const;

    QString buildUserActionLog(Action action, Device device = NONE, const QString& button = ""
                                , const QList<double>& values = QList<double>()
                                , const QStringList& strings = QStringList()) const;

    QString buildSystemEventLog(Event event, Device device = NONE, const QString& timerName = ""
            , int errorCode = 0, int level = 0, const QList<double>& values = QList<double>()
            , const QStringList& strings = QStringList()) const;

    QString buildHardwareInteractionLog(const QString& functionName, const QList<double>& values = QList<double>()
            , const QStringList& strings = QStringList()) const;

    QString buildGeneralLog(const QString& msg) const;

private:
    Logger(QObject *parent = 0);

    void addExtraFields(const QList<double>& values, const QStringList& strings, QString& extraFields
                  , QString& extraFieldValues) const;

    static Logger* _instance;

    QSqlDatabase _localDB;

    QSqlQuery _localDBQuery;

    MainWindowInterface* _mainWindow;

    static const QString _tapLogTemplate;
    static const QString _userActionLogTemplate;
    static const QString _systemEventLogTemplate;
    static const QString _hardwareInteractionLogTemplate;
    static const QString _generalLogTemplate;

    static const QString _actionNames[];
    static const QString _deviceNames[];
    static const QString _eventNames[];
};
