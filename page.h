#pragma once

#include <QWidget>
#include <QList>

#include "mainWindow.h"
#include "configManager.h"
#include "stateMachine.h"

// base class for pages
class Page : public QWidget {

public:
    Page(QWidget *parent) : QWidget(parent) {}

    virtual ~Page() {}

    virtual void init(MainWindow* mainWindow)
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
    virtual void onEntry()
    {

    }

private:
    // set page interface elements texts
    // @param mainWidget - central page QWidget
    void setTexts(QWidget* mainWidget, const QList<Text>& texts);

protected:
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

    MainWindow* _mainWindow;
};
