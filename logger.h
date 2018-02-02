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
        , BILLACCEPTOR
    };

    enum Event {
        SYSTEM_CHECK
        , USER_TIMER_START
        , USER_TIMER_STOP
        , SYSTEM_TIMER_START
        , SYSTEM_TIMER_STOP
        , ERROR_EVENT
        , INITIALIZATION_START
        , INITIALIZATION_FINISH
        , CONFIG_LOAD_START
        , CONFIG_LOAD_FINISH
        , STATE_TRANSITION_BEGIN
        , STATE_TRANSITION_END
        , STATE_CHANGED
        , STATE_MACHINE_START
        , ALCOGRAM_START
        , ALCOGRAM_FINISH
        , PAGE_ENTRANCE_START
        , PAGE_ENTRANCE_END
        , ALCOTEST_START
        , ALCOTEST_END
        , PERSON_ALCOTEST_INIT_START
        , PERSON_ALCOTEST_INIT_END
        , ALCOTEST_SUCCESS
        , ALCOTEST_FAIL
        , PRINTING_PHOTOS_START
        , PRINTING_PHOTOS_END
        , PHOTO_PRINT_SUCCESS
        , PHOTO_PRINT_FAIL
        , PAGE_EXIT_START
        , PAGE_EXIT_END
    };


    ~Logger();

    static Logger* instance();
    static void resetInstance();

    void setMainWindow(MainWindowInterface* mainWindow);

    // write log to databases
    // @param query - INSERT query
    void log(const QString& query);

    // build INSERT query for 'tapLog' table
    // @param x - x position of tap
    // @param y - y position of tap
    QString buildTapLog(int x, int y) const;

    // build INSERT query for 'userActionLog' table
    // @param button - name of button
    // @param value - extra numeric parameters
    // @param strings - extra string parameters
    QString buildUserActionLog(Action action, Device device = NONE, const QString& button = ""
                                , const QList<double>& values = QList<double>()
                                , const QStringList& strings = QStringList()) const;

    // build INSERT query for 'systemEventLog' table
    // @param timerName - used for timer events
    // @param level - importance of system event
    // @param value - extra numeric parameters
    // @param strings - extra string parameters
    QString buildSystemEventLog(Event event, Device device = NONE, const QString& timerName = ""
            , int errorCode = 0, int level = 0, const QList<double>& values = QList<double>()
            , const QStringList& strings = QStringList()) const;
    QString buildSystemEventLog(Event event, int errorCode, int level, const QStringList& strings = QStringList()) const;
    QString buildSystemEventLog(Event event, int errorCode, int level, const QList<double>& values = QList<double>()) const;

    // build INSERT query for 'hardwareInteractionLog' table
    // @param functionName - name of called function
    // @param value - extra numeric parameters
    // @param strings - extra string parameters
    QString buildHardwareInteractionLog(const QString& functionName, const QList<double>& values = QList<double>()
            , const QStringList& strings = QStringList()) const;

    // build INSER query for 'generalLog' table
    // @param msg - message
    QString buildGeneralLog(const QString& msg) const;

private:
    Logger(QObject *parent = 0);

    // add extram fields for INSERT query
    // @param values - numeric field values
    // @param strings - string field values
    // @param extraFields - string for writing names of fields
    // @param extraFieldValues - string for writing fields values
    void addExtraFields(const QList<double>& values, const QStringList& strings, QString& extraFields
                  , QString& extraFieldValues) const;

    static Logger* _instance;

    QSqlDatabase _localDB; // local log database
    QSqlQuery _localDBQuery; // QSqlQuery for local log database

    MainWindowInterface* _mainWindow;

    // INSERT query templates
    static const QString _tapLogTemplate; // for 'tapLog' table
    static const QString _userActionLogTemplate; // for 'actionLog' table
    static const QString _systemEventLogTemplate; // for 'systemEventLog' table
    static const QString _hardwareInteractionLogTemplate; // for 'hardwareInteraction' table
    static const QString _generalLogTemplate; // for 'generalLog' table

    // string values for enums
    static const QString _actionNames[]; // enum Action
    static const QString _deviceNames[]; // enum Device
    static const QString _eventNames[]; // enum Event
};
