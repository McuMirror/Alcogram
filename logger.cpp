#include <QDateTime>
#include <QVariant>
#include <QSqlError>
#include <QtGlobal>

#include "logger.h"
#include "utils.h"

const QString Logger::_tapLogTemplate = "INSERT INTO tapLog "
                                        "(machine_id, date_time, state, x, y) "
                                        "VALUES ('@machine_id', '@time', '@state', @x, @y)";

const QString Logger::_userActionLogTemplate = "INSERT INTO userAction "
                                               "(machine_id, date_time, state, action, device, button@EXTRA_FIELDS) "
                                               "VALUES ('@machine_id', '@time', '@state', '@action', '@device'"
                                               ", '@button'@EXTRA_VALUES)";

const QString Logger::_systemEventLogTemplate = "INSERT INTO systemEvent "
                                                "(machine_id, date_time, state, event_type, device, timer_name, "
                                                "event_id, error_code, level@EXTRA_FIELDS) "
                                                "VALUES ('@machine_id', '@date_time', '@state', '@event_type', "
                                                "'@device', '@timer_name', @event_id, @error_code, '@level'"
                                                "@EXTRA_VALUES)";

const QString Logger::_hardwareInteractionLogTemplate = "INSERT INTO hardwareInteraction "
                                                        "(machine_id, date_time, state, func_name@EXTRA_FIELDS) "
                                                        "VALUES ('@machine_id', '@time', '@state', '@func_name'"
                                                        "@EXTRA_VALUES)";

const QString Logger::_generalLogTemplate = "INSERT INTO general "
                                            "(machine_id, date_time, state, msg) "
                                            "VALUES ('@machine_id', '@time', '@state', '@msg')";

const QString Logger::_actionNames[] = {"BUTTON RELEASE"};
const QString Logger::_deviceNames[] = {"", "BUTTON", "ALCOTESTER", "CAMERA", "POS", "PRINTER"};
const QString Logger::_eventNames[] = {"SYSTEM CHECK", "USER TIMER START", "USER TIMER STOP"
                                      , "SYSTEM TIMER START", "SYSTEM TIMER STOP", "ERROR", "INITIALIZATION START"
                                      , "INITIALIZATION FINISH", "CONFIG LOAD START", "CONFIG LOAD FINISH"
                                      , "STATE TRANSITION BEGIN", "STATE TRANSITION END", "STATE CHANGED"
                                      , "STATE MACHINE START", "ALCOGRAM START", "ALCOGRAM FINISH"
                                      , "PAGE ENTRANCE START", "PAGE ENTRANCE END"
                                      , "ALCOTEST START", "ALCOTEST END", "PERSON ALCOTEST INIT START"
                                      , "PERSON ALCOTEST INIT END", "ALCOTEST SUCCESS", "ALCOTEST FAIL"
                                      , "PRINTING PHOTOS START", "PRINTING PHOTOS END", "PHOTO PRINT SUCCESS"
                                      , "PHOTO PRINT FAIL"};

Logger* Logger::_instance = NULL;

Logger::Logger(QObject* parent)
    : QObject(parent)
{
    _localDB = QSqlDatabase::addDatabase("QSQLITE");
    _localDB.setDatabaseName("./logs.db");
    _localDB.open();

    _localDBQuery = QSqlQuery(_localDB);

    _localDBQuery.exec("CREATE TABLE tapLog"
                    "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "machine_id TEXT, "
                    "date_time TEXT,"
                    "state TEXT,"
                    "x INTEGER,"
                    "y INTEGER)"
                    );

    _localDBQuery.exec("CREATE TABLE userAction"
                      "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "machine_id TEXT, "
                      "date_time TEXT,"
                      "state TEXT,"
                      "action TEXT,"
                      "device TEXT,"
                      "button TEXT,"
                      "val1 REAL,"
                      "val2 REAL,"
                      "val3 REAL,"
                      "val4 REAL,"
                      "val5 REAL,"
                      "str1 TEXT,"
                      "str2 TEXT,"
                      "str3 TEXT,"
                      "str4 TEXT,"
                      "str5 TEXT)"
                       );

    _localDBQuery.exec("CREATE TABLE systemEvent"
                       "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "machine_id TEXT, "
                       "date_time TEXT,"
                       "state TEXT,"
                       "event_type TEXT,"
                       "device TEXT,"
                       "timer_name TEXT,"
                       "event_id INTEGER,"
                       "error_code INTEGER,"
                       "level TEXT,"
                       "val1 REAL,"
                       "val2 REAL,"
                       "val3 REAL,"
                       "val4 REAL,"
                       "val5 REAL,"
                       "str1 TEXT,"
                       "str2 TEXT,"
                       "str3 TEXT,"
                       "str4 TEXT,"
                       "str5 TEXT)"
                       );

    _localDBQuery.exec("CREATE TABLE hardwareInteraction"
                       "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "machine_id TEXT, "
                       "date_time TEXT,"
                       "state TEXT,"
                       "func_name TEXT,"
                       "par1 REAL,"
                       "par2 REAL,"
                       "par3 REAL,"
                       "par4 REAL,"
                       "par5 REAL,"
                       "par6 REAL,"
                       "par7 REAL,"
                       "par8 REAL,"
                       "par9 REAL,"
                       "par10 REAL,"
                       "str1 TEXT,"
                       "str2 TEXT,"
                       "str3 TEXT,"
                       "str4 TEXT,"
                       "str5 TEXT,"
                       "str6 TEXT,"
                       "str7 TEXT,"
                       "str8 TEXT,"
                       "str9 TEXT,"
                       "str10 TEXT)"
                       );

    _localDBQuery.exec("CREATE TABLE environment"
                       "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "machine_id TEXT, "
                       "date_time TEXT,"
                       "state TEXT,"
                       "printer_state_id INTEGER,"
                       "alcotester_state_id INTEGER,"
                       "pos_state_id INTEGER,"
                       "camera_state_id INTEGER,"
                       "internet_connection_status_id INTEGER,"
                       "local_db_status_id INTEGER,"
                       "remote_db_status_id INTEGER,"
                       "memory_allocated INTEGER,"
                       "memory_free INTEGER,"
                       "last_time_user_was_active TEXT)"
                       );

    _localDBQuery.exec("CREATE TABLE general"
                       "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "machine_id TEXT, "
                       "date_time TEXT,"
                       "state TEXT,"
                       "msg TEXT)"
                       );
}

Logger::~Logger()
{
}

Logger* Logger::instance()
{
    if (_instance == NULL) {
        _instance = new Logger();
    }

    return _instance;
}

void Logger::resetInstance()
{
    delete _instance;
    _instance = NULL;
}

void Logger::setMainWindow(MainWindowInterface *mainWindow)
{
    _mainWindow = mainWindow;
}

void Logger::log(const QString& query)
{
    bool b = _localDBQuery.exec(query);
    QSqlError e = _localDBQuery.lastError();
    b = false;
}

QString Logger::buildTapLog(int x, int y) const
{
    QString query(_tapLogTemplate);

    query.replace("@machine_id", "123"); // TODO: get machineId from ConfigManager
    query.replace("@time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    query.replace("@state", Utils::getStateNameNumber(_mainWindow->getCurrentStateName()));
    query.replace("@x", QString::number(x));
    query.replace("@y", QString::number(y));

    return query;
}

QString Logger::buildUserActionLog(Action action, Device device, const QString& button
                                   , const QList<double>& values, const QStringList& strings) const
{
    QString extraFields = "";
    QString extraFieldValues = "";
    QString query(_userActionLogTemplate);

    query.replace("@machine_id", "123"); // TODO: get machineId from ConfigManager
    query.replace("@time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    query.replace("@state", Utils::getStateNameNumber(_mainWindow->getCurrentStateName()));
    query.replace("@action", _actionNames[action]);
    query.replace("@device", _deviceNames[device]);
    query.replace("@button", button);

    addExtraFields(values, strings, extraFields, extraFieldValues);

    query.replace("@EXTRA_FIELDS", extraFields);
    query.replace("@EXTRA_VALUES", extraFieldValues);

    return query;
}

QString Logger::buildSystemEventLog(Event event, Device device, const QString& timerName
    , int errorCode, int level, const QList<double>& values, const QStringList& strings) const
{
    QString extraFields = "";
    QString extraFieldValues = "";
    QString query(_systemEventLogTemplate);

    query.replace("@machine_id", "123"); // TODO: get machineId from ConfigManager
    query.replace("@date_time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    query.replace("@state", Utils::getStateNameNumber(_mainWindow->getCurrentStateName()));
    query.replace("@event_type", _eventNames[event]);
    query.replace("@device", _deviceNames[device]);
    query.replace("@timer_name", timerName);
    query.replace("@event_id", QString::number(static_cast<int>(event)));
    query.replace("@error_code", QString::number(errorCode));
    query.replace("@level", QString::number(level));

    addExtraFields(values, strings, extraFields, extraFieldValues);

    query.replace("@EXTRA_FIELDS", extraFields);
    query.replace("@EXTRA_VALUES", extraFieldValues);

    return query;
}

QString Logger::buildSystemEventLog(Event event, int errorCode, int level, const QStringList &strings) const
{
    return buildSystemEventLog(event, NONE, "", errorCode, level, QList<double>(), strings);
}

QString Logger::buildSystemEventLog(Event event, int errorCode, int level, const QList<double> &values) const
{
    return buildSystemEventLog(event, NONE, "", errorCode, level, values, QStringList());
}

QString Logger::buildHardwareInteractionLog(const QString& functionName, const QList<double>& values
                                            , const QStringList& strings) const
{
    QString extraFields = "";
    QString extraFieldValues = "";
    QString query(_hardwareInteractionLogTemplate);

    query.replace("@machine_id", "123"); // TODO: get machineId from ConfigManager
    query.replace("@time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    query.replace("@state", Utils::getStateNameNumber(_mainWindow->getCurrentStateName()));
    query.replace("@func_name", functionName);

    addExtraFields(values, strings, extraFields, extraFieldValues);

    query.replace("@EXTRA_FIELDS", extraFields);
    query.replace("@EXTRA_VALUES", extraFieldValues);

    return query;
}

QString Logger::buildGeneralLog(const QString& msg) const
{
    QString query(_generalLogTemplate);

    query.replace("@machine_id", "123"); // TODO: get machineId from ConfigManager
    query.replace("@time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    query.replace("@state", Utils::getStateNameNumber(_mainWindow->getCurrentStateName()));
    query.replace("@msg", msg);

    return query;
}

void Logger::addExtraFields(const QList<double>& values, const QStringList& strings, QString& extraFields
                      , QString& extraFieldValues) const
{
    for (int i = 0; i < values.size(); i++) {
        extraFields += ", val" + QString::number(i + 1);
        extraFieldValues += ", " + QString::number(values.at(i));
    }

    for (int i = 0; i < strings.size(); i++) {
        extraFields += ", str" + QString::number(i + 1);
        extraFieldValues += ", '" + strings.at(i) + "'";
    }
}

