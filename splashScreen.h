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

private:
    Ui::SplashScreen *_ui;
};
