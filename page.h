#pragma once

#include <QWidget>
#include <QList>

#include "mainWindow.h"
#include "configManager.h"

class Page : public QWidget {

public:
    Page(QWidget *parent) : QWidget(parent) {}

    virtual ~Page() {}

    virtual void init(MainWindow* mainWindow) {
        _mainWindow = mainWindow;
    }

    virtual QString getName() const{
        return "";
    }

private:
    void setTexts(QWidget* mainWidget, const QList<Text>& texts);

protected:
    void updateTexts(QWidget* mainWidget, bool onlyLanguageDependent = false);

    MainWindow* _mainWindow;
};
