#pragma once

#include <QWidget>

#include "page.h"

namespace Ui {
    class SplashScreen;
}

class SplashScreen : public Page
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = 0);
    ~SplashScreen();

    void init(MainWindow* mainWindow) override;
    QString getName() const override;
    QList<Transition*> getTransitions() override;

protected:
    void onEntry();

private:
    void onDevicesStarted();
    void onSomeDevicesNotStarted();

    bool _firstLaunch = true;
    Ui::SplashScreen *_ui;
};
