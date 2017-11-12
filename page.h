#pragma once

#include <QWidget>
#include <QList>

#include "mainWindow.h"
#include "configManager.h"

// base class for pages
class Page : public QWidget {

public:
    Page(QWidget *parent) : QWidget(parent) {}

    virtual ~Page() {}

    virtual void init(MainWindow* mainWindow)
    {
        _mainWindow = mainWindow;
    }

    // get page name
    virtual QString getName() const
    {
        return "";
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

    MainWindow* _mainWindow;
};
