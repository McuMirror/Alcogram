#pragma once

#include <QWidget>

#include "page.h"

namespace Ui {
    class SplashScreenNoncriticalError;
}

class SplashScreenNoncriticalError : public Page
{
    Q_OBJECT

public:
    explicit SplashScreenNoncriticalError(QWidget *parent = 0);
    ~SplashScreenNoncriticalError();

    void init(MainWindowInterface* mainWindow) override;
    QString getName() const override;

private:
    Ui::SplashScreenNoncriticalError *ui;
};
