#pragma once

#include <QWidget>

#include "page.h"

namespace Ui {
    class SplashScreenEternalSleep;
}

class SplashScreenEternalSleep : public Page
{
    Q_OBJECT

public:
    explicit SplashScreenEternalSleep(QWidget *parent = 0);
    ~SplashScreenEternalSleep();

    void init(MainWindowInterface* mainWindow) override;
    QString getName() const override;

private:
    Ui::SplashScreenEternalSleep *ui;
};
