#pragma once

#include <QWidget>
#include <QList>
#include <QTimer>

#include "interfaces/mainwindowinterface.h"
#include "configManager.h"
#include "stateMachine.h"

// base class for pages
class Page : public QWidget {

public:
    Page(QWidget *parent) : QWidget(parent) {}

    virtual ~Page() {}

    virtual void init(MainWindowInterface* mainWindow)
    {
        _mainWindow = mainWindow;

        initInterface();
        setConnections();
    }

    // get page name
    virtual QString getName() const
    {
        return "";
    }

    // get state machine transitions for this page
    virtual QList<Transition*> getTransitions()
    {
        return QList<Transition*>();
    }

    // called when page becomes active
    void enter();

    // called when page becomes inactive
    void exit();

    // switch page interface language
    void switchLanguage();

private:
    // set page interface elements texts
    // @param mainWidget - central page QWidget
    void setTexts(QWidget* mainWidget, const QList<Text>& texts);

protected:
    // @param timerName - used for logging
    // @param timeS - timer duration in seconds, used for logging
    void startTimer(const QString &timerName, int timeS);
    void stopTimer();

    // called from enter()
    virtual void onEntry()
    {

    }

    virtual void onExit()
    {

    }

    // update page interface elements texts
    // @param mainWidget - central page QWidget
    // @param onlyLanguageDependent - update only language dependent text
    void updateTexts(QWidget* mainWidget, bool onlyLanguageDependent = false);

    // init interface widgets if need
    virtual void initInterface()
    {

    }

    // set all page signal-slot connections
    virtual void setConnections()
    {

    }

    MainWindowInterface* _mainWindow;
    QString _currentTimerName = ""; // the name of active timer
    QTimer _timer;
};
