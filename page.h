#pragma once

#include <QWidget>

#include "mainWindow.h"

class Page : public QWidget {

public:
    Page(QWidget *parent) : QWidget(parent) {}

    virtual ~Page() {}

    virtual void init(MainWindow* mainWindow) {
        Q_UNUSED(mainWindow);
    }
};
